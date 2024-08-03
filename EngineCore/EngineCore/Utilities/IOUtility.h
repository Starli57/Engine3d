#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Core/Ref.h"

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;

    void FindAndEmplaceResourcesFiles(
        const std::string& rootFolderPath,
        const std::vector<std::string>& extensions,
        std::unordered_map<std::string, std::filesystem::path>& result);
};

