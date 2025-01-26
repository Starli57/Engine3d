#pragma once

#include "AssetsLoader.h"
#include "AssetsDatabaseVulkan.h"

class AssetsLoaderVulkan : public AssetsLoader
{
public:
	AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase);

	void LoadAllTextures() override;
	void UnLoadAllTextures() override;

	void LoadAllMeshes(std::vector<MeshMeta>& meshes) override;
	void UnLoadAllMeshes() override;
};