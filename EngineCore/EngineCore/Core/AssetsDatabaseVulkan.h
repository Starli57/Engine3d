#pragma once

#include "AssetsDatabase.h"

namespace EngineCore
{
	class AssetsDatabaseVulkan : public AssetsDatabase
	{
	public:
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
	
		AssetsDatabaseVulkan(const Ref<ProjectSettings>& projectSettings);
		~AssetsDatabaseVulkan() override;
	};
}