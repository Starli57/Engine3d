#include "EngineCore/Pch.h"
#include "ResourcesConverterGltf.h"
#include "EngineCore/Serialization/EntitySerializer.h"

#include <tiny_gltf.h>

void ResourcesConverterGltf::ImportMesh(const std::string& meshPathStr, const std::filesystem::path& meshPath,
    const std::string& inFolder, const std::string& outFolder, const std::string& directoryName,
    std::unordered_map<std::string, std::filesystem::path>& texturesPaths)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF gltf_ctx;
    std::string err;
    std::string warn;
    
    bool ret = gltf_ctx.LoadASCIIFromFile(&model, &err, &warn, meshPathStr.c_str());

    if (!warn.empty()) spdlog::warn(warn);
    if (!err.empty()) spdlog::error(err);

    if (!ret)
    {
        spdlog::critical("Failed to parse glTF by path: {}", meshPathStr);
        return;
    }
    
}
