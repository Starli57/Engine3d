#pragma once

#include "AssetsLoader.h"
#include "ResourcesStorageVulkan.h"

namespace EngineCore
{
	class AssetsLoaderVulkan : public AssetsLoader
	{
	public:
		AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<ResourcesStorageVulkan> assetsDatabase);

		void LoadTexture(const std::filesystem::path& path) override;
		void LoadTexture(const std::filesystem::path& path, uint32_t textureIndex) override;
		void UnLoadAllTextures() override;

		void LoadMesh(std::filesystem::path& path) override;
		void SetupMeshBuffers(MeshMeta& meshMeta, uint32_t meshIndex) override;
		void UnLoadAllMeshes() override;

		void LoadMaterial(std::filesystem::path& path) override;
		void UnLoadAllMaterial() override;
		
		//todo: replace and make via a pattern
		URef<std::mutex> graphicsQueueMutex;

		AVulkan::GraphicsApiVulkan* vulkanApi;
		std::shared_ptr<ResourcesStorageVulkan> assetsDatabaseVulkan;

	};
}