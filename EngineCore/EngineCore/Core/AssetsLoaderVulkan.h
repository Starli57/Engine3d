#pragma once

#include "AssetsLoader.h"
#include "AssetsDatabaseVulkan.h"

class AssetsLoaderVulkan : public AssetsLoader
{
public:
	AssetsLoaderVulkan(Ref<ProjectSettings> projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase);
	~AssetsLoaderVulkan() override;

	void LoadAllTextures() override;
	void UnLoadAllTextures() override;

	void LoadAllMeshes(std::vector<MeshMeta>& meshes) override;
	void UnLoadAllMeshes() override;
};