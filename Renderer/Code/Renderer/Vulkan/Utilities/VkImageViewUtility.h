#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class VkImageViewUtility
	{
	public:
		static void Create(VkDevice& logicalDevice, VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags, 
			VkImage& image, VkImageView& imageView);
		static void Destroy(VkDevice& logicalDevice, VkImageView& imageView);
	};
}