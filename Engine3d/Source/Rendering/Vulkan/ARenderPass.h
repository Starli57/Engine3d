#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class ARenderPass
	{
	public:
		VkRenderPass Create(VkDevice& logicalDevice, VkFormat& scImageFormat) const;
		void Dispose(VkDevice& logicalDevice, VkRenderPass& renderPass) const;
	};
}