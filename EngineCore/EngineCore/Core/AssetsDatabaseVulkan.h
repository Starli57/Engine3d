#pragma once

#include "AssetsDatabase.h"

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

	
    AssetsDatabaseVulkan(const Ref<ProjectSettings>& projectSettings);
	~AssetsDatabaseVulkan() override;
};

