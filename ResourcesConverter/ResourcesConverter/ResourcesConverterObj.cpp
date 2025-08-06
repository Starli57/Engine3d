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
    std::vector<ConvertingMeshData>* meshes = new std::vector<ConvertingMeshData>();
    meshes->resize(materials.size());

    for (int i = 0; i < materials.size(); i++)
    {
        meshes->at(i).materialIndex = i;
        meshes->at(i).materialPath = outFolder + ToLowerCase(materials.at(i).name);
        meshes->at(i).materialName = ToLowerCase(materials.at(i).name);
    }

    for (const auto& shape : shapes)
    {
        uint32_t i = 0;
        for (const auto& index : shape.mesh.indices)
        {
            auto materialId = shape.mesh.material_ids[i / 3];
            ConvertingMeshData& meshMeta = meshes->at(materialId);

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
        auto assetName = ToLowerCase(fileNameWithoutExtension + "_" + meshIt.materialName);

        if (meshIt.vertices.empty() || meshIt.indices.empty())
        {
            spdlog::warn("Mesh {} has {} vertices and {} indices -> it will not be serialized", assetName, meshIt.vertices.size(), meshIt.indices.size());
            return;
        }

        // Serialize mesh to binary
        auto serialized = SerializeMesh(outFolder + assetName + ".mesh", meshIt);
        if (serialized)
        {
            serializedMeshNames.at(meshIt.materialIndex) = outFolder + assetName + ".mesh";
            spdlog::info("Mesh {} saved to folder {}", assetName, outFolder);
        }
            
        //serialize material to yaml
        YAML::Node materialNode;

        auto materialName = ToLowerCase(material.name);
        bool isOpaque = material.dissolve >= 1.0 && material.alpha_texname.empty();
        materialNode["materialName"] = materialName;
        materialNode["pipelineName"] = isOpaque ? "opaque" : "transparent";
        materialNode["isOpaque"] = isOpaque;
            
        materialNode["roughness"] = material.roughness;
        materialNode["metallic"] = material.metallic;
        materialNode["sheen"] = material.sheen;
        materialNode["specularExponent"] = material.shininess;
        materialNode["indexOfRefraction"] = material.ior;
        materialNode["transparency"] = material.dissolve;
            
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

        auto saveMaterialPath = outFolder + materialName + ".material";
        std::ofstream fMaterialOut(saveMaterialPath);
        fMaterialOut << materialNode;
        fMaterialOut.close();
        serializedMaterialNames.at(meshIt.materialIndex) = saveMaterialPath;
    });

    std::for_each(std::execution::par, texturesPaths.begin(), texturesPaths.end(), [this, &inFolder, &outFolder](const auto& pathIter)
    {
        auto textureName = ToLowerCase(FormatTextureName(pathIter.second.string(), inFolder));
        std::filesystem::copy(pathIter.second.string(), outFolder + textureName, std::filesystem::copy_options::overwrite_existing);
    });

    WriteSerializedMeshNames(outFolder + directoryName + "_" + meshName +  ".combinedmesh", serializedMeshNames, serializedMaterialNames);
        
    //clean
    delete meshes;
}