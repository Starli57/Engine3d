#include "EngineCore/Pch.h"
#include "IOUtility.h"
#include "spdlog/spdlog.h"

std::vector<char> IOUtility::ReadFile(const std::string& filepath) const
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    const std::filesystem::path fullPath = std::filesystem::relative(filepath);
    CAssert::Check(file.is_open(), "Failed to open file " + absolute(fullPath).string());

    const size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void IOUtility::FindResourcesFiles(const std::string& rootFolderPath, std::unordered_map<std::filesystem::path, uint32_t>& paths) const
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootFolderPath))
    {
        if (!entry.is_regular_file()) continue;

        if (paths.find(entry.path()) != paths.end())
        {
            spdlog::warn("File with the same path has been added already: {}" + entry.path().string());
            continue;
        }

        paths.emplace(entry.path(), 1);
    }
}

void IOUtility::FindAndEmplaceResourcesFiles(
    const std::string& rootFolderPath, 
    const std::vector<std::string>& extensions,
    std::vector<std::filesystem::path>& paths,
    std::unordered_map<std::filesystem::path, uint32_t>& indexByPath) const
{
    uint32_t i = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootFolderPath)) 
    {
        if (!entry.is_regular_file()) continue;

        auto entryPath = entry.path();
        auto extension = entryPath.filename().extension().string();

        if (indexByPath.find(entryPath) != indexByPath.end())
        {
            spdlog::warn("File with the same path has been added already: {}" + entryPath.string());
            continue;
        }

        for (const auto& relevantExtension : extensions)
        {
            if (relevantExtension != extension) continue;

            paths.push_back(entryPath);
            indexByPath.emplace(entryPath, i);
            
            i++;
            break;
        }
    }
}
