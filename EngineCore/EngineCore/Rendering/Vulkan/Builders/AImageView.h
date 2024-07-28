#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Rollback/Rollback.h"

namespace AVulkan
{
	class AImageView
	{
	public:
		void Create(VkDevice& logicalDevice, VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
			VkImage& image, VkImageView& imageView, Ref<Rollback> rollback);
	};
}