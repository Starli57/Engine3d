#include "IOUtility.h"
#include "spdlog/spdlog.h"

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

void IOUtility::FindResourcesFiles(
    const std::string& rootFolderPath, 
    const std::vector<std::string>& extensions, 
    std::vector<std::filesystem::path>& result)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootFolderPath)) 
    {
        if (!entry.is_regular_file()) continue;
        
        auto ext = entry.path().extension().string();
        for (auto relevantExtension : extensions)
        {
            if (relevantExtension != ext) continue;

            result.push_back(entry.path());
            break;
        }
    }
}
