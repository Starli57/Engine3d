#pragma once

#include "AssetsDatabase.h"
#include "EngineCore/Assets/Material.h"
#include "EngineCore/Assets/Meta/MeshMeta.h"
#include "EngineCore/Assets/Meta/MaterialMeta.h"
#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Core/Ref.h"

class AssetsLoader
{
public:
	AssetsLoader(Ref<ProjectSettings> projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase);
	virtual ~AssetsLoader() = default;

	void Load();

	uint32_t LoadTextureStr(const std::string& path);
	uint32_t LoadTexture(const std::filesystem::path& path);

	void PrepareAllMeshesMeta(std::vector<MeshMeta>& meshes);
	virtual void LoadAllMeshes(std::vector<MeshMeta>& meshes);
	virtual void UnLoadAllMeshes();

	virtual void LoadAllTextures();
	virtual void UnLoadAllTextures();

	void LoadAllMaterials();
	
protected:
	IGraphicsApi* graphicsApi;
	Ref<ProjectSettings> projectSettings;
	Ref<AssetsDatabase> assetsDatabase;
};

