#include "Pch.h"
#include "AssetsDatabase.h"
#include "EngineShared/IOUtility.h"

//todo: calculate texture links count to be able to dispose if not used

AssetsDatabase::AssetsDatabase(Ref<ProjectSettigns> projectSettings)
{
	this->projectSettings = projectSettings;

	textures = std::vector<Ref<Texture>>();

	FillMeshesPaths();
	FillTexturesPaths();
}
	
AssetsDatabase::~AssetsDatabase()
{
	for (auto& texture : textures) {
		texture.reset();
	}

	textures.clear();
}

int32_t AssetsDatabase::TextureIndex(const std::filesystem::path& texturePath)
{
	int32_t index = 0;
	for (auto it = textures.begin(); it != textures.end(); ++it, ++index) 
	{
		if (it->get()->textureFilePath == texturePath) 
		{
			return index;
		}
	}
	return -1;
}

int32_t AssetsDatabase::TextureIndex(const UniqueId uniqueId)
{
	int32_t index = 0;
	for (auto it = textures.begin(); it != textures.end(); ++it, ++index)
	{
		if (it->get()->uniqueId == uniqueId)
		{
			return index;
		}
	}
	return -1;
}

bool AssetsDatabase::HasTexture(const std::filesystem::path& texturePath)
{
	return TextureIndex(texturePath) >= 0;
}

Ref<Texture> AssetsDatabase::GetTexture(const int32_t index)
{
	CAssert::Check(index >= 0 && index < textures.size(), "Texture index is out of range");
	return textures[index];
}

Ref<Texture> AssetsDatabase::GetTexture(const std::filesystem::path& texturePath)
{
	auto textureIndex = TextureIndex(texturePath);
	CAssert::Check(textureIndex >= 0, "Assets database doesn't have a texture with path: " + texturePath.string());
	return textures[textureIndex];
}

int32_t AssetsDatabase::AddTexture(Ref<Texture> texture)
{
	auto textureIndex = TextureIndex(texture->textureFilePath);
	if (textureIndex >= 0) return textureIndex;

	textures.push_back(texture);
	return textures.size() - 1;
}

void AssetsDatabase::RemoveTexture(const std::filesystem::path& texturePath)
{
	auto textureIndex = TextureIndex(texturePath);
	CAssert::Check(textureIndex >= 0, "Assets database doesn't have a texture with path: " + texturePath.string());
	textures.erase(textures.begin() + textureIndex);
}

int32_t AssetsDatabase::MaterialIndex(const UniqueId uniqueId)
{
	int32_t index = 0;
	for (auto it = materials.begin(); it != materials.end(); ++it, ++index)
	{
		if (it->get()->uniqueId == uniqueId)
		{
			return index;
		}
	}
	return -1;
}

Ref<Material> AssetsDatabase::GetMaterial(const int32_t index)
{
	CAssert::Check(index >= 0 && index < textures.size(), "Material index is out of range");
	return materials[index];
}

int32_t AssetsDatabase::AddMaterial(Ref<Material> material)
{
	materials.push_back(material);
	return materials.size() - 1;
}

void AssetsDatabase::RemoveMaterial(Ref<Material> material)
{
	textures.erase(textures.begin() + MaterialIndex(material->uniqueId));
}

void AssetsDatabase::FillMeshesPaths()
{
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".obj");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, meshesPaths);
}

void AssetsDatabase::FillTexturesPaths()
{
	auto relevantExtensions = std::vector<std::string>();
	relevantExtensions.reserve(1);
	relevantExtensions.push_back(".png");
	IOUtility().FindAndEmplaceResourcesFiles(projectSettings->resourcesPath, relevantExtensions, texturesPaths);
}

