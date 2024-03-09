#include "Pch.h"

#include <filesystem>

#include "IOUtility.h"
#include "spdlog/spdlog.h"


//todo: return a pointer instead
std::vector<char> IOUtility::ReadFile(const std::string& filepath) const
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    std::filesystem::path fullPath = std::filesystem::relative(filepath);
    CAssert::Check(file.is_open(), "Failed to open file " + std::filesystem::absolute(fullPath).string());

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}