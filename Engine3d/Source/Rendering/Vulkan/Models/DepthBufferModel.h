#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	struct DepthBufferModel 
	{
		Ref<VkImage> image;
		Ref<VkDeviceMemory> imageMemory;
		Ref<VkImageView> imageView;
	};
}