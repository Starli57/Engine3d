#pragma once
#include <tiny_gltf.h>

#include "ConvertingMeshData.h"
#include "ResourcesConverter.h"

class ResourcesConverterGltf : public ResourcesConverter
{
public:
    void ImportFolder(const std::string& inFolder, const std::string& outFolder, const std::string& directoryName);
    void ImportMesh(const std::string& meshPathStr, const std::filesystem::path& meshPath, const std::string& inFolder, 
        const std::string& outFolder, const std::string& directoryName, std::unordered_map<std::string, std::filesystem::path>& texturesPaths);

private:
    void ImportMaterials(tinygltf::Model& model, const std::string& outFolder,
        std::vector<ConvertingMeshData>* meshes, std::vector<std::string>& serializedMaterialNames) const;
};
