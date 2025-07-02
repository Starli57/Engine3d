#pragma once

#include "AssetsLoader.h"
#include "AssetsDatabaseVulkan.h"

namespace EngineCore
{
	class AssetsLoaderVulkan : public AssetsLoader
	{
	public:
		AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<AssetsDatabaseVulkan> assetsDatabase);

		void LoadTexture(const std::filesystem::path& path) override;
		void LoadTexture(const std::filesystem::path& path, uint32_t textureIndex) override;
		void LoadAllTextures() override;
		void UnLoadAllTextures() override;

		void LoadMesh(std::filesystem::path& path) override;
		void SetupMesh(MeshMeta& meshMeta, uint32_t meshIndex) override;
		void LoadAllMeshes() override;
		void UnLoadAllMeshes() override;

		//todo: replace and make via a pattern
		URef<std::mutex> graphicsQueueMutex;

		AVulkan::GraphicsApiVulkan* vulkanApi;
		std::shared_ptr<AssetsDatabaseVulkan> assetsDatabaseVulkan;

	};
}