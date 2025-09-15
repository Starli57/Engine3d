#pragma once

#include <GLFW/glfw3.h>

namespace VulkanApi
{
	struct ImageModel
	{
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	};
}