#pragma once

#include "ResourcesStorage.h"

namespace EngineCore
{
	class ResourcesStorageVulkan : public ResourcesStorage
	{
	public:
		std::vector<VkImage> images;
		std::vector<VkImageView> imagesViews;
		std::vector<VkDeviceMemory> imagesMemory;
		
		std::vector<VkBuffer> materialTransparencyBuffers;
		std::vector<VkDeviceMemory> materialTransparencyBuffersMemory;
		
		std::vector<VkBuffer> vertexBuffers;
		std::vector<VkDeviceMemory> vertexBuffersMemory;

		std::vector<VkBuffer> indexBuffers;
		std::vector<VkDeviceMemory> indexBuffersMemory;

		std::vector<uint32_t> indexesCount;

		std::vector<glm::vec3> boundingBoxMin;
		std::vector<glm::vec3> boundingBoxMax;
		std::vector<glm::vec3> boundingBoxCenter;
	
		ResourcesStorageVulkan(const Ref<ProjectSettings>& projectSettings);
		~ResourcesStorageVulkan() override;
	};
}