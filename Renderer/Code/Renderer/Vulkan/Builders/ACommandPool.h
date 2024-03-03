#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class ACommandPool
	{
	public:
		VkCommandPool Create(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface) const;
		void Dispose(VkDevice& logicalDevice, VkCommandPool& commandPool) const;
	};
}