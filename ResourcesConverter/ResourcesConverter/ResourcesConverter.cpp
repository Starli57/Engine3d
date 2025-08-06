#include "EngineCore/Pch.h"
#include "ResourcesConverter.h"
#include "EngineCore/Utilities/YamlConverters.h"

#include <fstream>

void ResourcesConverter::FillPaths(std::unordered_map<std::string, std::filesystem::path>& paths, 
                                   const std::vector<std::string>& extensions, const std::string& inFolder) const
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(inFolder))
    {
        if (!entry.is_regular_file()) continue;

        auto entryPath = entry.path();
        auto fileName = ToLowerCase(entryPath.filename().string());
        auto extension = entryPath.extension().string();

        if (paths.find(fileName) != paths.end())
        {
            spdlog::error("File with the same name was already added: " + fileName);
            continue;
        }

        for (const auto& relevantExtension : extensions)
        {
            if (relevantExtension != extension) continue;

            paths.emplace(fileName, entryPath);
            break;
        }
    }
}

void ResourcesConverter::BindTextureToMaterial(YAML::Node& node, const std::string& materialKey, const std::string& outFolder, std::string& rawTexturePath) const
{
    if (!rawTexturePath.empty())
    {
        node[materialKey] = outFolder + ToLowerCase(rawTexturePath.substr(rawTexturePath.find_last_of("/\\") + 1));
    }
}

void ResourcesConverter::CopyTextureImage(std::string& rawTexturePath, const std::string& inFolder, const std::string& assetFolder) const
{
    if (!rawTexturePath.empty())
    {
        auto texturePath = FormatTexturePath(rawTexturePath, inFolder);
        auto textureName = ToLowerCase(FormatTextureName(rawTexturePath, inFolder));
        std::filesystem::copy(texturePath, assetFolder + "/" + textureName, std::filesystem::copy_options::overwrite_existing);
    }
}

std::string ResourcesConverter::FormatTexturePath(const std::string& texturePath, const std::string& inFolder) const
{
    return inFolder + texturePath.substr(texturePath.find_first_of("/\\"));
}

std::string ResourcesConverter::FormatTextureName(const std::string& texturePath, const std::string& inFolder) const
{
    return texturePath.substr(texturePath.find_last_of("/\\") + 1);
}

std::string ResourcesConverter::ToLowerCase(const std::string& input) const
{
    std::string result = input;
    std::ranges::transform(result, result.begin(), [](const unsigned char c)
    {
        return std::tolower(c);
    });
    return result;
}

void ResourcesConverter::CreateFolder(const std::filesystem::path& path) const
{
    if (!std::filesystem::exists(path))
    {
        std::filesystem::create_directories(path);
    }
}

//todo:don't need to serialize materials names as it exist in meshes already
void ResourcesConverter::WriteSerializedMeshNames(const std::string& filePath, const std::vector<std::string>&  meshes, const std::vector<std::string>& materials) const
{
    std::ofstream fOut(filePath);

    auto count = meshes.size();
    for(int i = 0; i < count; i++)
    {
        fOut << meshes.at(i) + "\n";
        fOut << materials.at(i) + "\n";
    }
    
    fOut.close();
}

bool ResourcesConverter::SerializeMesh(const std::string& filePath, const ConvertingMeshData& meshIt) const
{
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile)
    {
        spdlog::critical("Failed to open file for writing mesh data");
        return false;
    }

    size_t vertexCount = meshIt.vertices.size();
    outFile.write(reinterpret_cast<const char*>(&vertexCount), sizeof(size_t));
    outFile.write(reinterpret_cast<const char*>(meshIt.vertices.data()), vertexCount * sizeof(EngineCore::Vertex));

    size_t indexCount = meshIt.indices.size();
    outFile.write(reinterpret_cast<const char*>(&indexCount), sizeof(size_t));
    outFile.write(reinterpret_cast<const char*>(meshIt.indices.data()), indexCount * sizeof(uint32_t));

    auto materialPath = meshIt.materialPath + ".material";
    size_t materialPathSize = materialPath.size();
    outFile.write(reinterpret_cast<const char*>(&materialPathSize), sizeof(materialPathSize));
    outFile.write(materialPath.data(), materialPathSize);

    auto materialName = ToLowerCase(meshIt.materialName + ".material");
    size_t materialNameSize = materialName.size();
    outFile.write(reinterpret_cast<const char*>(&materialNameSize), sizeof(materialNameSize));
    outFile.write(materialName.data(), materialNameSize);
    
    outFile.close();

    return true;
}
