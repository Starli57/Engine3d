#pragma once
#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkFormat FindSupportedFormat(VkPhysicalDevice& physicalDevice, std::vector<VkFormat>& formats,
			VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat FindDepthBufferFormat(VkPhysicalDevice& physicalDevice);
}