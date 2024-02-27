#include "Pch.h"

#include <filesystem>

#include "IOUtility.h"
#include "spdlog/spdlog.h"


//todo: return a pointer instead
std::vector<char> IOUtility::ReadFile(const std::string& filepath) const
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    std::filesystem::path fullPath = std::filesystem::absolute(filepath);
    spdlog::debug("Read file by path: {0}", fullPath.string());

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file " + filepath);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

//todo: make async
Ref<stbi_uc> IOUtility::LoadTexture(const std::string& filepath, int* width, int* height, int* channels, int channelsFormat)
{
    auto pixels = stbi_load(filepath.c_str(), width, height, channels, channelsFormat);
    return CreateRef<stbi_uc>(pixels);
}
