#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class AImageView
	{
	public:
		void Create(VkDevice& logicalDevice, VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
			VkImage& image, VkImageView& imageView);
		void Destroy(VkDevice& logicalDevice, VkImageView& imageView);
	};
}