#pragma once

#include "AssetsDatabase.h"
#include "EngineCore/Assets/Material.h"
#include "EngineCore/Assets/Meta/MeshMeta.h"
#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Core/Ref.h"

class AssetsLoader
{
public:
	AssetsLoader(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, const Ref<AssetsDatabase>& assetsDatabase);
	virtual ~AssetsLoader() = default;

	void Load();

	uint32_t GetTextureStr(const std::string& path) const;
	uint32_t GetTexture(const std::filesystem::path& path) const;
	
	uint32_t GetOrLoadTextureStr(const std::string& path);
	uint32_t GetOrLoadTexture(const std::filesystem::path& path);

	void DeserializeMeshMeta(const std::filesystem::path& path, MeshMeta& meshMeta) const;

	virtual void LoadMesh(std::filesystem::path& path) = 0;
	virtual void SetupMesh(MeshMeta& meshMeta, uint32_t meshIndex) = 0;
	virtual void LoadAllMeshes() = 0;
	virtual void UnLoadAllMeshes() = 0;

	virtual void LoadTexture(const std::filesystem::path& path) = 0;
	virtual void LoadTexture(const std::filesystem::path& path, uint32_t textureIndex) = 0;
	virtual void LoadAllTextures() = 0;
	virtual void UnLoadAllTextures() = 0;

	void LoadMaterial(const std::filesystem::path& path, uint32_t index);
	void LoadAllMaterials();

	void LoadRequestedMeshes();
	void LoadRequestedMaterials();

protected:
	IGraphicsApi* graphicsApi;
	Ref<ProjectSettings> projectSettings;
	Ref<AssetsDatabase> assetsDatabase;
};

