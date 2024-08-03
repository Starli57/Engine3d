#include "EngineCore/Pch.h"
#include "EngineCore/AssetsManagement/AssetsDatabase.h"
#include "EngineCore/Utilities/IOUtility.h"

//todo: calculate assets links count to be able to dispose if not used

AssetsDatabase::AssetsDatabase(Ref<ProjectSettigns> projectSettings)
{
	this->projectSettings = projectSettings;

	FillMeshesPaths();
	FillTexturesPaths();
}
	
AssetsDatabase::~AssetsDatabase()
{
	//todo: check which resources need to dispose here
	for (auto& texture : textures) texture.reset();

	textures.clear();
}

int32_t AssetsDatabase::MeshIndex(const UniqueId uniqueId)
{
	return GetIndex(meshes, uniqueId);
}

int32_t AssetsDatabase::MaterialIndex(const UniqueId uniqueId)
{
	return GetIndex(materials, uniqueId);
}

int32_t AssetsDatabase::TextureIndex(const UniqueId uniqueId)
{
	return GetIndex(textures, uniqueId);
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

Ref<Mesh> AssetsDatabase::GetMesh(const int32_t index)
{
	CAssert::Check(index >= 0 && index < meshes.size(), "Mesh index is out of range");
	return meshes[index];
}

Ref<Material> AssetsDatabase::GetMaterial(const int32_t index)
{
	CAssert::Check(index >= 0 && index < textures.size(), "Material index is out of range");
	return materials[index];
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

int32_t AssetsDatabase::AddMesh(Ref<Mesh> mesh)
{
	//todo: check if the mesh is already in the list
	meshes.push_back(mesh);
	return meshes.size() - 1;
}

int32_t AssetsDatabase::AddMaterial(Ref<Material> material)
{
	materials.push_back(material);
	return materials.size() - 1;
}

int32_t AssetsDatabase::AddTexture(Ref<Texture> texture)
{
	auto textureIndex = TextureIndex(texture->textureFilePath);
	if (textureIndex >= 0) return textureIndex;

	textures.push_back(texture);
	return textures.size() - 1;
}

void AssetsDatabase::RemoveMesh(Ref<Mesh> mesh)
{
	meshes.erase(meshes.begin() + MeshIndex(mesh->uniqueId));
}

void AssetsDatabase::RemoveMaterial(Ref<Material> material)
{
	materials.erase(materials.begin() + MaterialIndex(material->uniqueId));
}

void AssetsDatabase::RemoveTexture(Ref<Texture> texture)
{
	textures.erase(textures.begin() + TextureIndex(texture->uniqueId));
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

