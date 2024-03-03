#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class ALogicalDevice
	{
	public:
		VkDevice Create(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface,
			VkQueue& graphicsQueue, VkQueue& presentationQueue) const;

		void Dispose(VkDevice& logicalDevice) const;
	};
}