#pragma once
#include <GLFW/glfw3.h>

namespace VulkanApi
{
	VkFormat FindSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& formats,
	                             VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat FindDepthBufferFormat(const VkPhysicalDevice& physicalDevice);
}