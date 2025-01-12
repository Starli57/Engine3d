#pragma once

#include "ResourcesConverter.h"

class ResourcesConverterObj : public ResourcesConverter
{
public:
	void Convert(const std::string& inFolder, const std::string& outFolder) override;

	void FillPaths(std::unordered_map<std::string, std::filesystem::path>& paths, const std::vector<std::string>& extensions, const std::string& inFolder);

	void BindTextureToMaterial(YAML::Node& node, const std::string& materialKey, const std::string& outFolder, std::string& rawTexturePath);
	void CopyTextureImage(std::string& rawTexturePath, const std::string& inFolder, const std::string& assetFolder);

	std::string FormatTexturePath(const std::string& texturePath, const std::string& inFolder);
	std::string FormatTextureName(const std::string& texturePath, const std::string& inFolder);

	bool SerializeMesh(const std::string& filePath, MeshMeta& meshIt);
	std::string ToLowerCase(const std::string& input);

};