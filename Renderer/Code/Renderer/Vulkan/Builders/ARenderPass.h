#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class ARenderPass
	{
	public:
		VkRenderPass Create(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkFormat& scImageFormat) const;
		void Dispose(VkDevice& logicalDevice, VkRenderPass& renderPass) const;
	};
}