#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Rollback/Rollback.h"

namespace AVulkan
{
	class AImageView
	{
	public:
		void Create(VkDevice& logicalDevice, VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
			VkImage& image, VkImageView& imageView, Ref<Rollback> rollback);
	};
}