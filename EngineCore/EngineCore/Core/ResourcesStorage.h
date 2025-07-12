#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <mutex>
#include <queue>

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Utilities/UniqueId.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Assets/Material.h"
#include "EngineCore/Assets/Meta/MeshMeta.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rendering/Vertex.h"

namespace EngineCore
{
	class ResourcesStorage
	{
	public:
		// 0 - unloaded, 1 - in progress, 2 - ready
		std::vector<uint16_t> meshLoadStatuses;
		std::vector<uint16_t> materialLoadStatuses;
		std::vector<uint16_t> textureLoadStatuses;

		std::queue<uint32_t> meshLoadRequests;
		std::queue<uint32_t> materialLoadRequests;
		std::queue<uint32_t> textureLoadRequests;

		std::mutex meshLoadStatusMutex;
		std::mutex materialLoadStatusMutex;
		std::mutex textureLoadStatusMutex;
	
		std::vector<std::filesystem::path> meshesPaths;
		std::vector<std::filesystem::path> materialsPaths;
		std::vector<std::filesystem::path> prefabsPaths;
		std::vector<std::filesystem::path> texturesPaths;
		std::vector<std::filesystem::path> worldsPaths;
		std::vector<std::filesystem::path> combinedMeshPaths;

		std::unordered_map<std::filesystem::path, uint32_t> meshesIndexByPath;
		std::unordered_map<std::filesystem::path, uint32_t> materialsIndexByPath;
		std::unordered_map<std::filesystem::path, uint32_t> prefabsIndexByPath;
		std::unordered_map<std::filesystem::path, uint32_t> texturesIndexByPath;
		std::unordered_map<std::filesystem::path, uint32_t> worldsIndexByPath;
		std::unordered_map<std::filesystem::path, uint32_t> combinedMeshIndexByPath;

		std::unordered_map<uint32_t, MeshMeta> customMeshes;
	
		std::vector<Ref<Material>> materials;
		std::vector<uint32_t> meshMaterialBinding;
	
		ResourcesStorage(const Ref<ProjectSettings>& projectSettings);
		virtual ~ResourcesStorage() = default;

		size_t MeshesCount() const { return meshesPaths.size(); }
		size_t MaterialsCount() const { return materials.size(); }
		size_t TexturesCount() const { return texturesPaths.size(); }

		bool SetMeshLoadingStatus(const std::filesystem::path& path);
		bool SetMaterialLoadingStatus(const std::filesystem::path& path);
		bool SetTextureLoadingStatus(const std::filesystem::path& path);
	
		void AddMeshLoadRequest(const std::filesystem::path& path);
		void AddMaterialLoadRequest(const std::filesystem::path& path);
		void AddTextureLoadRequest(const std::filesystem::path& path);
	
	private:
		Ref<ProjectSettings> projectSettings;

		void FillAssetsPaths();
		void FillMeshesPaths();
		void FillMaterialsPaths();
		void FillPrefabsPaths();
		void FillTexturesPaths();
		void FillWorldsPaths();
		void FillCustomMeshes();
	};
}