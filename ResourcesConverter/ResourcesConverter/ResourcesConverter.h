#pragma once
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <string>
#include <unordered_map>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"
#include "ConvertingMeshData.h"

class ResourcesConverter
{
public:
    void ImportFolder(const std::string& inFolder, const std::string& outFolder, const std::string& directoryName);
    void ImportMesh(const std::string& meshPathStr, const std::filesystem::path& meshPath, const std::string& inFolder, 
        const std::string& outFolder, const std::string& directoryName, std::unordered_map<std::string, std::filesystem::path>& texturesPaths);
    bool SerializeMesh(const std::string& filePath, const ConvertingMeshData& meshIt) const;

    void CreateFolder(const std::filesystem::path& folderPath) const;
    void FillPaths(std::unordered_map<std::string, std::filesystem::path>& paths, const std::vector<std::string>& extensions, const std::string& inFolder) const;

    void BindTextureToMaterial(YAML::Node& node, const std::string& materialKey, const std::string& outFolder, std::string& rawTexturePath) const;
    void CopyTextureImage(std::string& rawTexturePath, const std::string& inFolder, const std::string& assetFolder) const;

    std::string FormatTexturePath(const std::string& texturePath, const std::string& inFolder) const;
    std::string FormatTextureName(const std::string& texturePath, const std::string& inFolder) const;

    void WriteSerializedMeshNames(const std::string& filePath, const std::vector<std::string>& meshes, const std::vector<std::string>& materials) const;
    std::string ToLowerCase(const std::string& input) const;
};
