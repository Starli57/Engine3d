#include "EngineCore/Pch.h"
#include "ResourcesConverterObj.h"
#include "EngineCore/Serialization/EntitySerializer.h"
#include "EngineCore/Utilities/YamlConverters.h"
#include <execution>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "ConvertingMeshData.h"

void ResourcesConverterObj::ImportFolder(const std::string& inFolder, const std::string& outFolder, const std::string& directoryName)
{
    auto outParentPath = std::filesystem::path(outFolder).parent_path();
    CreateFolder(outParentPath);
    
    std::unordered_map<std::string, std::filesystem::path> meshesPaths;
    std::unordered_map<std::string, std::filesystem::path> texturesPaths;

    auto meshesExtensions = std::vector<std::string>();
    meshesExtensions.reserve(1);
    meshesExtensions.emplace_back(".obj");

    auto texturesExtensions = std::vector<std::string>();
    texturesExtensions.reserve(3);
    texturesExtensions.emplace_back(".png");
    texturesExtensions.emplace_back(".jpg");
    texturesExtensions.emplace_back(".JPG");

    FillPaths(meshesPaths, meshesExtensions, inFolder);
    FillPaths(texturesPaths, texturesExtensions, inFolder);

    for (const auto& meshPath : meshesPaths)
    {
        ImportMesh(meshPath.first, meshPath.second, inFolder, outFolder, directoryName, texturesPaths);
    }

    spdlog::info("Imported mesh: {}", inFolder);
}

void ResourcesConverterObj::ImportMesh(const std::string& meshPathStr, const std::filesystem::path& meshPath, const std::string& inFolder, 
    const std::string& outFolder, const std::string& directoryName, std::unordered_map<std::string, std::filesystem::path>& texturesPaths)
{
    //parse mesh
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    auto fileName = meshPath.filename();
    auto meshName = fileName.string().substr(0, fileName.string().find_last_of("."));
    auto pathString = meshPath.string();
    auto baseDirectory = pathString.substr(0, pathString.find_last_of("/\\") + 1);
    auto isLoaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, pathString.c_str(), baseDirectory.c_str());

    if (!warn.empty()) spdlog::warn(warn);
    if (!err.empty()) spdlog::error(err);
    if (!isLoaded)
    {
        spdlog::error("Failed to load obj");
        return;
    }
        
    //format meshes
    auto meshesMap = new std::unordered_map<uint16_t, std::unordered_map<uint16_t, ConvertingMeshData>>();

    for(uint16_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
    {
        if (!meshesMap->contains(shapeIndex)) meshesMap->emplace(shapeIndex, std::unordered_map<uint16_t, ConvertingMeshData>());
        
        auto shape = shapes[shapeIndex];
        uint16_t i = 0;
        
        for (const auto& index : shape.mesh.indices)
        {
            auto materialIndex = shape.mesh.material_ids[i / 3];
            if (!meshesMap->at(shapeIndex).contains(materialIndex)) meshesMap->at(shapeIndex).emplace(materialIndex, ConvertingMeshData());
            
            ConvertingMeshData& meshMeta = meshesMap->at(shapeIndex).at(materialIndex);

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

    auto meshes = new std::vector<ConvertingMeshData>();
    uint16_t meshIndexCounter = 0;
    for(const auto& meshesByShape : *meshesMap)
    {
        for(const auto& meshesByMaterial : meshesByShape.second)
        {
            ConvertingMeshData mesh = meshesByMaterial.second;
            mesh.meshIndex = meshIndexCounter++;
            mesh.materialIndex = meshesByMaterial.first;
            mesh.materialPath = outFolder + ToLowerCase(materials.at(mesh.materialIndex).name);
            mesh.materialName = ToLowerCase(materials.at(mesh.materialIndex).name);
            meshes->push_back(mesh);
        }
    }

    //todo: fix tangents calculation
    //CalculateNormalsTangents(meshes);
        
    //--format meshes
    std::vector<std::string> serializedMeshNames;
    std::vector<std::string> serializedMaterialNames;
    serializedMeshNames.resize(meshes->size());
    serializedMaterialNames.resize(meshes->size());
        
    std::for_each(std::execution::par, meshes->begin(), meshes->end(),
        [this, &materials, &meshPathStr,  &inFolder, &outFolder, &serializedMeshNames, &serializedMaterialNames](ConvertingMeshData& meshIt)
    {
        auto material = materials.at(meshIt.materialIndex);
        auto fileNameWithoutExtension = meshPathStr.substr(0, meshPathStr.find_last_of("."));
        auto assetName = ToLowerCase(fileNameWithoutExtension + "_" + meshIt.materialName + std::to_string(meshIt.meshIndex));

        if (meshIt.vertices.empty() || meshIt.indices.empty())
        {
            spdlog::warn("Mesh {} has {} vertices and {} indices -> it will not be serialized", assetName, meshIt.vertices.size(), meshIt.indices.size());
            return;
        }

        // Serialize mesh to binary
        auto serialized = SerializeMesh(outFolder + assetName + ".mesh", meshIt);
        if (serialized)
        {
            serializedMeshNames.at(meshIt.meshIndex) = outFolder + assetName + ".mesh";
            spdlog::info("Mesh {} saved to folder {}", assetName, outFolder);
        }
    });

    for(int i = 0; i < materials.size(); i++)
    {
        //serialize material to yaml
        YAML::Node materialNode;
        auto material = materials.at(i);
        auto materialName = ToLowerCase(material.name);
        auto transparency = material.dissolve;
        auto isOpaque = transparency >= 1.0 && material.alpha_texname.empty();
        
        materialNode["materialName"] = materialName;
        materialNode["pipelineName"] = isOpaque ? "opaque" : "transparent";
            
        materialNode["roughness"] = material.roughness;
        materialNode["metallic"] = material.metallic;
        materialNode["sheen"] = material.sheen;
        materialNode["specularExponent"] = material.shininess;
        materialNode["indexOfRefraction"] = material.ior;
        materialNode["transparency"] = transparency;
        materialNode["isOpaque"] = isOpaque;
        
        materialNode["ambientColor"] = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
        materialNode["diffuseColor"] = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        materialNode["specularColor"] = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
        materialNode["emissionColor"] = glm::vec3(material.emission[0], material.emission[1], material.emission[2]);
            
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
    }

    //todo:don't need to serialize materials names as it exist in meshes already
    for(int i = 0; i < meshes->size(); i++)
    {
        auto material = materials.at(meshes->at(i).materialIndex);
        auto materialName = ToLowerCase(material.name);
        serializedMaterialNames.at(i) = outFolder + materialName + ".material";
    }
    
    std::for_each(std::execution::par, texturesPaths.begin(), texturesPaths.end(), [this, &inFolder, &outFolder](const auto& pathIter)
    {
        auto textureName = ToLowerCase(FormatTextureName(pathIter.second.string(), inFolder));
        std::filesystem::copy(pathIter.second.string(), outFolder + textureName, std::filesystem::copy_options::overwrite_existing);
    });

    WriteSerializedMeshNames(outFolder + directoryName + "_" + meshName +  ".combinedmesh", serializedMeshNames, serializedMaterialNames);
        
    //clean
    delete meshes;
}

bool ResourcesConverterObj::SerializeMesh(const std::string& filePath, const ConvertingMeshData& meshIt) const
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

//todo:don't need to serialize materials names as it exist in meshes already
void ResourcesConverterObj::WriteSerializedMeshNames(const std::string& filePath, const std::vector<std::string>&  meshes, const std::vector<std::string>& materials) const
{
    std::ofstream fOut(filePath);

    auto count = meshes.size();
    for(int i = 0; i < count; i++)
    {
        fOut << meshes.at(i) + "\n";
        fOut << materials.at(i) + "\n";
    }
    
    fOut.close();
}

void ResourcesConverterObj::FillPaths(std::unordered_map<std::string, std::filesystem::path>& paths, 
                                      const std::vector<std::string>& extensions, const std::string& inFolder) const
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

        for (const auto& relevantExtension : extensions)
        {
            if (relevantExtension != extension) continue;

            paths.emplace(fileName, entryPath);
            break;
        }
    }
}

void ResourcesConverterObj::BindTextureToMaterial(YAML::Node& node, const std::string& materialKey, const std::string& outFolder, std::string& rawTexturePath) const
{
    if (!rawTexturePath.empty())
    {
        node[materialKey] = outFolder + ToLowerCase(rawTexturePath.substr(rawTexturePath.find_last_of("/\\") + 1));
    }
}

void ResourcesConverterObj::CopyTextureImage(std::string& rawTexturePath, const std::string& inFolder, const std::string& assetFolder) const
{
    if (!rawTexturePath.empty())
    {
        auto texturePath = FormatTexturePath(rawTexturePath, inFolder);
        auto textureName = ToLowerCase(FormatTextureName(rawTexturePath, inFolder));
        std::filesystem::copy(texturePath, assetFolder + "/" + textureName, std::filesystem::copy_options::overwrite_existing);
    }
}

std::string ResourcesConverterObj::FormatTexturePath(const std::string& texturePath, const std::string& inFolder) const
{
    return inFolder + texturePath.substr(texturePath.find_first_of("/\\"));
}

std::string ResourcesConverterObj::FormatTextureName(const std::string& texturePath, const std::string& inFolder) const
{
    return texturePath.substr(texturePath.find_last_of("/\\") + 1);
}

std::string ResourcesConverterObj::ToLowerCase(const std::string& input) const
{
    std::string result = input;
    std::ranges::transform(result, result.begin(), [](const unsigned char c)
    {
        return std::tolower(c);
    });
    return result;
}

void ResourcesConverterObj::CreateFolder(const std::filesystem::path& path) const
{
    if (!std::filesystem::exists(path))
    {
        std::filesystem::create_directories(path);
    }
}
