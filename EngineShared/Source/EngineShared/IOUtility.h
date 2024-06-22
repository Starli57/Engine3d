#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "EngineShared/CustomAssert.h"
#include "EngineShared/Ref.h"

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;

    void FindResourcesFiles(
        const std::string& rootFolderPath,
        const std::vector<std::string>& extensions,
        std::vector<std::filesystem::path>& result);
};

