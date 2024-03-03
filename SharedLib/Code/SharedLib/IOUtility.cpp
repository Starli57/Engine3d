#include "spdlog/spdlog.h"

#include <filesystem>

#include "IOUtility.h"


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