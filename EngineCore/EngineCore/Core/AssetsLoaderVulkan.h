#pragma once

#include "ResourcesStorageVulkan.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"

namespace Engine
{
	class AssetsLoaderVulkan
	{
	public:
		AssetsLoaderVulkan(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, Ref<ResourcesStorageVulkan> assetsDatabase);
		~AssetsLoaderVulkan() = default;
		
		void Load();

		uint32_t GetTextureStr(const std::string& path) const;
		uint32_t GetTexture(const std::filesystem::path& path) const;
	
		uint32_t GetOrLoadTextureStr(const std::string& path);
		uint32_t GetOrLoadTexture(const std::filesystem::path& path);

		void LoadAndDeserializeMesh(const std::filesystem::path& path, MeshAsset& meshAsset) const;
		void LoadMesh(std::filesystem::path& path);
		void SetupMeshBuffers(MeshAsset& meshAsset, uint32_t meshIndex);
		void UnLoadAllMeshes();

		void LoadTexture(const std::filesystem::path& path);
		void LoadTexture(const std::filesystem::path& path, uint32_t textureIndex);
		void UnLoadAllTextures();

		void LoadAndDeserializeMaterial(const std::filesystem::path& path, const Ref<PbrMaterialAsset>& material);
		void LoadMaterial(std::filesystem::path& path);
		void UnLoadAllMaterial();
		
		void LoadRequestedMeshes();
		void LoadRequestedMaterials();
		
		//todo: replace and make via a pattern
		URef<std::mutex> graphicsQueueMutex;

		AVulkan::GraphicsApiVulkan* vulkanApi;
		Ref<ResourcesStorageVulkan> assetsDatabase;
		Ref<ProjectSettings> projectSettings;

	};
}
