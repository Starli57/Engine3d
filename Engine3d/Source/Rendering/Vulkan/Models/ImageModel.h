#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	struct ImageModel
	{
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	};
}