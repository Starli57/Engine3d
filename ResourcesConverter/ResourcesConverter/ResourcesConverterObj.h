#pragma once

#include "ResourcesConverter.h"

class ResourcesConverterObj : public ResourcesConverter
{
public:
	void Convert(const std::string& inFolder, const std::string& outFolder, const std::string& directoryName) override;

	void FillPaths(std::unordered_map<std::string, std::filesystem::path>& paths, const std::vector<std::string>& extensions, const std::string& inFolder) const;

	void BindTextureToMaterial(YAML::Node& node, const std::string& materialKey, const std::string& outFolder, std::string& rawTexturePath) const;
	void CopyTextureImage(std::string& rawTexturePath, const std::string& inFolder, const std::string& assetFolder) const;

	std::string FormatTexturePath(const std::string& texturePath, const std::string& inFolder) const;
	std::string FormatTextureName(const std::string& texturePath, const std::string& inFolder) const;

	bool SerializeMesh(const std::string& filePath, const MeshMeta& meshIt) const;
	void WriteSerializedMeshNames(const std::string& filePath, const std::vector<std::string>& meshes, const std::vector<std::string>& materials) const;
	std::string ToLowerCase(const std::string& input) const;

};