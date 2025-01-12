#include "EngineCore/Pch.h"
#include "ResourcesConverterObj.h"
#include "EngineCore/Serialization/EntitySerializer.h"

#include <execution>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void ResourcesConverterObj::Convert(const std::string& inFolder, const std::string& outFolder)
{
    auto outParentPath = std::filesystem::path(outFolder).parent_path();

    if (!std::filesystem::exists(outParentPath))
    {
        std::filesystem::create_directories(outParentPath);
    }
    
    std::unordered_map<std::string, std::filesystem::path> meshesPaths;
    std::unordered_map<std::string, std::filesystem::path> texturesPaths;

    auto meshesExtensions = std::vector<std::string>();
    meshesExtensions.reserve(1);
    meshesExtensions.push_back(".obj");

    auto texturesExtensions = std::vector<std::string>();
    texturesExtensions.reserve(3);
    texturesExtensions.push_back(".png");
    texturesExtensions.push_back(".jpg");
    texturesExtensions.push_back(".JPG");

    FillPaths(meshesPaths, meshesExtensions, inFolder);
    FillPaths(texturesPaths, texturesExtensions, inFolder);

    for (const auto& meshPath : meshesPaths)
    {
        //parse mesh
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        auto pathString = meshPath.second.string();
        auto baseDirectory = pathString.substr(0, pathString.find_last_of("/\\") + 1);
        auto isLoaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, pathString.c_str(), baseDirectory.c_str());

        if (!warn.empty()) spdlog::warn(warn);
        if (!err.empty()) spdlog::error(err);
        if (!isLoaded) spdlog::error("Failed to load obj");


        //format meshes
        std::vector<MeshMeta>* meshes = new std::vector<MeshMeta>();
        meshes->resize(materials.size());

        for (int i = 0; i < materials.size(); i++)
        {
            meshes->at(i).materialIndex = i;
            meshes->at(i).materialPath = outFolder + ToLowerCase(materials.at(i).name);
            meshes->at(i).materialName = ToLowerCase(materials.at(i).name);
            meshes->at(i).vertices.reserve(10000);
            meshes->at(i).indices.reserve(10000);
        }

        for (const auto& shape : shapes)
        {
            uint32_t i = 0;
            for (const auto& index : shape.mesh.indices)
            {
                auto materialId = shape.mesh.material_ids[i / 3];
                MeshMeta& meshMeta = meshes->at(materialId);

                Vertex vertex;
                vertex.position =
                {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                if (index.normal_index != -1)
                {
                    vertex.normal =
                    {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index != -1)
                {
                    vertex.uv =
                    {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                vertex.color = { 1.0f, 1.0f, 1.0f };

                meshMeta.vertices.push_back(vertex);
                meshMeta.indices.push_back(static_cast<uint32_t>(meshMeta.indices.size()));
                i++;
            }
        }

        for (auto meshIt = meshes->begin(); meshIt < meshes->end(); meshIt++)
        {
            for (auto index = 0; index < meshIt->indices.size(); index += 3)
            {
                glm::vec3 edge1 = meshIt->vertices.at(index + 1).position - meshIt->vertices.at(index).position;
                glm::vec3 edge2 = meshIt->vertices.at(index + 2).position - meshIt->vertices.at(index).position;
                glm::vec2 deltaUV1 = meshIt->vertices.at(index + 1).uv - meshIt->vertices.at(index).uv;
                glm::vec2 deltaUV2 = meshIt->vertices.at(index + 2).uv - meshIt->vertices.at(index).uv;

                float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                meshIt->vertices.at(index).tangent += glm::vec3(
                    f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
                    f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
                    f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z));

                meshIt->vertices.at(index).bitangent += glm::vec3(
                    f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
                    f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
                    f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z));

                meshIt->vertices.at(index + 1).tangent += glm::vec3(
                    f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
                    f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
                    f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z));

                meshIt->vertices.at(index + 1).bitangent += glm::vec3(
                    f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
                    f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
                    f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z));

                meshIt->vertices.at(index + 2).tangent += glm::vec3(
                    f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
                    f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
                    f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z));

                meshIt->vertices.at(index + 2).bitangent += glm::vec3(
                    f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
                    f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
                    f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z));
            }
        }
        //--format meshes

        std::for_each(std::execution::par, meshes->begin(), meshes->end(), [this, &materials, &meshPath,  &inFolder, &outFolder](MeshMeta& meshIt)
        {
            auto material = materials.at(meshIt.materialIndex);
            auto fileNameWithoutExtension = meshPath.first.substr(0, meshPath.first.find_last_of("."));
            auto assetName = ToLowerCase(fileNameWithoutExtension + "_" + meshIt.materialName);

            if (meshIt.vertices.size() == 0 || meshIt.indices.size() == 0)
            {
                spdlog::warn("Mesh {} has {} vertices and {} indices -> it will not be serialized", assetName, meshIt.vertices.size(), meshIt.indices.size());
                return;
            }

            // Serialize mesh to binary
            auto serialized = SerializeMesh(outFolder + assetName + ".mesh", meshIt);
            if (serialized) spdlog::info("Mesh {} saved to folder {}", assetName, outFolder);

            //serialize material to yaml
            YAML::Node materialNode;

            auto materialName = ToLowerCase(material.name);
            materialNode["materialName"] = materialName;
            materialNode["pipelineName"] = "albedoOpaque";

            BindTextureToMaterial(materialNode, "ambientTextureName", outFolder, material.ambient_texname);
            BindTextureToMaterial(materialNode, "diffuseTextureName", outFolder, material.diffuse_texname);
            BindTextureToMaterial(materialNode, "specularTextureName", outFolder, material.specular_texname);
            BindTextureToMaterial(materialNode, "specular_highlightTextureName", outFolder, material.specular_highlight_texname);
            BindTextureToMaterial(materialNode, "bumpTextureName", outFolder, material.bump_texname);
            BindTextureToMaterial(materialNode, "displacementTextureName", outFolder, material.displacement_texname);
            BindTextureToMaterial(materialNode, "alphaTextureName", outFolder, material.alpha_texname);
            BindTextureToMaterial(materialNode, "reflectionTextureName", outFolder, material.reflection_texname);

            std::ofstream fMaterialOut(outFolder + materialName + ".material");
            fMaterialOut << materialNode;
            fMaterialOut.close();
        });

        std::for_each(std::execution::par, texturesPaths.begin(), texturesPaths.end(), [this, &inFolder, &outFolder](const auto& pathIter)
        {
            auto textureName = ToLowerCase(FormatTextureName(pathIter.second.string(), inFolder));
            std::filesystem::copy(pathIter.second.string(), outFolder + textureName, std::filesystem::copy_options::overwrite_existing);
        });

        //clean
        delete meshes;
    }
}

bool ResourcesConverterObj::SerializeMesh(const std::string& filePath, MeshMeta& meshIt)
{
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile)
    {
        spdlog::critical("Failed to open file for writing mesh data");
        return false;
    }

    size_t vertexCount = meshIt.vertices.size();
    outFile.write(reinterpret_cast<const char*>(&vertexCount), sizeof(size_t));
    outFile.write(reinterpret_cast<const char*>(meshIt.vertices.data()), vertexCount * sizeof(Vertex));

    size_t indexCount = meshIt.indices.size();
    outFile.write(reinterpret_cast<const char*>(&indexCount), sizeof(size_t));
    outFile.write(reinterpret_cast<const char*>(meshIt.indices.data()), indexCount * sizeof(uint32_t));

    auto materialPath = meshIt.materialPath + ".material";
    size_t materialPathSize = materialPath.size();
    outFile.write(reinterpret_cast<const char*>(&materialPathSize), sizeof(materialPathSize));
    outFile.write(materialPath.data(), materialPathSize);

    auto materialName = ToLowerCase(meshIt.materialName + ".material");
    size_t materialNameSize = materialName.size();
    outFile.write(reinterpret_cast<const char*>(&materialNameSize), sizeof(materialNameSize));
    outFile.write(materialName.data(), materialNameSize);
    
    outFile.close();

    return true;
}

void ResourcesConverterObj::FillPaths(std::unordered_map<std::string, std::filesystem::path>& paths, 
    const std::vector<std::string>& extensions, const std::string& inFolder)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(inFolder))
    {
        if (!entry.is_regular_file()) continue;

        auto entryPath = entry.path();
        auto fileName = ToLowerCase(entryPath.filename().string());
        auto extension = entryPath.extension().string();

        if (paths.find(fileName) != paths.end())
        {
            spdlog::error("File with the same name was already added: " + fileName);
            continue;
        }

        for (auto relevantExtension : extensions)
        {
            if (relevantExtension != extension) continue;

            paths.emplace(fileName, entryPath);
            break;
        }
    }
}

void ResourcesConverterObj::BindTextureToMaterial(YAML::Node& node, const std::string& materialKey, const std::string& outFolder, std::string& rawTexturePath)
{
    if (rawTexturePath.size() > 0)
    {
        node[materialKey] = outFolder + ToLowerCase(rawTexturePath.substr(rawTexturePath.find_last_of("/\\") + 1));
    }
}

void ResourcesConverterObj::CopyTextureImage(std::string& rawTexturePath, const std::string& inFolder, const std::string& assetFolder)
{
    if (rawTexturePath.size() > 0)
    {
        auto texturePath = FormatTexturePath(rawTexturePath, inFolder);
        auto textureName = ToLowerCase(FormatTextureName(rawTexturePath, inFolder));
        std::filesystem::copy(texturePath, assetFolder + "/" + textureName, std::filesystem::copy_options::overwrite_existing);
    }
}

std::string ResourcesConverterObj::FormatTexturePath(const std::string& texturePath, const std::string& inFolder)
{
    return inFolder + texturePath.substr(texturePath.find_first_of("/\\"));
}

std::string ResourcesConverterObj::FormatTextureName(const std::string& texturePath, const std::string& inFolder)
{
    return texturePath.substr(texturePath.find_last_of("/\\") + 1);
}

std::string ResourcesConverterObj::ToLowerCase(const std::string& input)
{
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
    {
        return std::tolower(c);
    });
    return result;
}