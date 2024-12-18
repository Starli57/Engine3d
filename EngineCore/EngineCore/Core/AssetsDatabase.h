#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Utilities/UniqueId.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Assets/Mesh.h"
#include "EngineCore/Assets/Material.h"
#include "EngineCore/Assets/Texture.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

class AssetsDatabase
{
public:
	std::unordered_map<std::string, std::filesystem::path> meshesPaths;
	std::unordered_map<std::string, std::filesystem::path> texturesPaths;
	std::unordered_map<std::string, std::filesystem::path> prefabsPaths;

	AssetsDatabase(Ref<ProjectSettigns> projectSettings);
	~AssetsDatabase();
	void Dispose();

	size_t MeshesCount() { return meshes.size(); }
	size_t MaterialsCount() { return materials.size(); }
	size_t TexturesCount() { return textures.size(); }

	int32_t MeshIndex(const UniqueId uniqueId);
	int32_t MaterialIndex(const UniqueId uniqueId);
	int32_t TextureIndex(UniqueId uniqueId);
	int32_t TextureIndex(const std::filesystem::path& texturePath);

	Ref<Mesh> GetMesh(const int32_t index);
	Ref<Material> GetMaterial(const uint32_t index);
	Ref<Texture> GetTexture(const int32_t index);
	Ref<Texture> GetTexture(const std::filesystem::path& texturePath);

	int32_t AddMesh(Ref<Mesh> mesh);
	int32_t AddTexture(Ref<Texture> texture);
	int32_t AddMaterial(Ref<Material> material);

	void RemoveMesh(Ref<Mesh> mesh);
	void RemoveMaterial(Ref<Material> material);
	void RemoveTexture(Ref<Texture> texture);

	template <typename T>
	int32_t GetIndex(std::vector<Ref<T>> assets, const UniqueId uniqueId)
	{
		int32_t index = 0;
		for (auto it = assets.begin(); it != assets.end(); ++it, ++index)
		{
			if (it->get()->uniqueId == uniqueId)
			{
				return index;
			}
		}
		return -1;
	}

private:
	Ref<ProjectSettigns> projectSettings;

	std::vector<Ref<Mesh>> meshes;
	std::vector<Ref<Material>> materials;
	std::vector<Ref<Texture>> textures;

	void FillMeshesPaths();
	void FillTexturesPaths();
	void FillPrefabsPaths();
};