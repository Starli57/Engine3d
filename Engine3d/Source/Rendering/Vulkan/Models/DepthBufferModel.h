#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	struct DepthBufferModel 
	{
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	};
}