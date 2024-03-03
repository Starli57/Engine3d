#pragma once
#include <GLFW/glfw3.h>

namespace AVulkan
{
	class VkFormatUtility
	{
	public:
		static VkFormat FindSupportedFormat(VkPhysicalDevice& physicalDevice, std::vector<VkFormat>& formats,
			VkImageTiling tiling, VkFormatFeatureFlags features);

		static VkFormat FindDepthBufferFormat(VkPhysicalDevice& physicalDevice);
	};
}