#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;

    void FindAndEmplaceResourcesFiles(
        const std::string& rootFolderPath,
        const std::vector<std::string>& extensions,
        std::vector<std::filesystem::path>& paths,
        std::unordered_map<std::filesystem::path, uint32_t>& indexByPath) const;
};

