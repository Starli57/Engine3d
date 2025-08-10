#pragma once

#include "ResourcesStorage.h"
#include "EngineCore/Assets/PbrMaterial.h"
#include "EngineCore/Assets/Meta/MeshMeta.h"
#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Core/Ref.h"

namespace EngineCore
{
	class AssetsLoader
	{
	public:
		AssetsLoader(const Ref<ProjectSettings>& projectSettings, IGraphicsApi* graphicsApi, const Ref<ResourcesStorage>& assetsDatabase);
		virtual ~AssetsLoader() = default;

		void Load();

		uint32_t GetTextureStr(const std::string& path) const;
		uint32_t GetTexture(const std::filesystem::path& path) const;
	
		uint32_t GetOrLoadTextureStr(const std::string& path);
		uint32_t GetOrLoadTexture(const std::filesystem::path& path);

		void LoadAndDeserializeMesh(const std::filesystem::path& path, MeshMeta& meshMeta) const;
		virtual void LoadMesh(std::filesystem::path& path) = 0;
		virtual void SetupMeshBuffers(MeshMeta& meshMeta, uint32_t meshIndex) = 0;
		virtual void UnLoadAllMeshes() = 0;

		virtual void LoadTexture(const std::filesystem::path& path) = 0;
		virtual void LoadTexture(const std::filesystem::path& path, uint32_t textureIndex) = 0;
		virtual void UnLoadAllTextures() = 0;

		void LoadAndDeserializeMaterial(const std::filesystem::path& path, const Ref<PbrMaterial>& material);
		virtual void LoadMaterial(std::filesystem::path& path) = 0;
		virtual void UnLoadAllMaterial() = 0;
		
		void LoadRequestedMeshes();
		void LoadRequestedMaterials();

	protected:
		IGraphicsApi* graphicsApi;
		Ref<ProjectSettings> projectSettings;
		Ref<ResourcesStorage> assetsDatabase;
	};
}
