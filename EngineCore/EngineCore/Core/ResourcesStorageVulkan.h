#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <unordered_map>
#include <filesystem>
#include "ProjectSettings.h"
#include "EngineCore/Assets/MeshAsset.h"
#include "EngineCore/Assets/PbrMaterialAsset.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"

namespace EngineCore
{
	class ResourcesStorageVulkan
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

		std::unordered_map<uint32_t, MeshAsset> customMeshes;
	
		std::vector<Ref<PbrMaterialAsset>> materials;
		std::vector<uint32_t> meshMaterialBinding;
	
		ResourcesStorageVulkan(const Ref<ProjectSettings>& projectSettings);
		virtual ~ResourcesStorageVulkan() = default;

		size_t MeshesCount() const { return meshesPaths.size(); }
		size_t MaterialsCount() const { return materials.size(); }
		size_t TexturesCount() const { return texturesPaths.size(); }

		bool SetMeshLoadingStatus(const std::filesystem::path& path);
		bool SetMaterialLoadingStatus(const std::filesystem::path& path);
		bool SetTextureLoadingStatus(const std::filesystem::path& path);
	
		void AddMeshLoadRequest(const std::filesystem::path& path);
		void AddMaterialLoadRequest(const std::filesystem::path& path);
		void AddTextureLoadRequest(const std::filesystem::path& path);
	
		std::vector<VkImage> images;
		std::vector<VkImageView> imagesViews;
		std::vector<VkDeviceMemory> imagesMemory;
		
		std::vector<VkBuffer> vertexBuffers;
		std::vector<VkDeviceMemory> vertexBuffersMemory;

		std::vector<VkBuffer> indexBuffers;
		std::vector<VkDeviceMemory> indexBuffersMemory;

		std::vector<uint32_t> indexesCount;

		std::vector<glm::vec3> boundingBoxMin;
		std::vector<glm::vec3> boundingBoxMax;
		std::vector<glm::vec3> boundingBoxCenter;
	
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
