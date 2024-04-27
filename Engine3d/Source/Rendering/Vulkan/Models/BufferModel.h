#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	struct BufferModel
	{
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* bufferMapped;
	};
}