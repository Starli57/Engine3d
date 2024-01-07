#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class AValidationLayers
	{
	public:
		void Setup(VkInstanceCreateInfo& createInfo);
		void Setup(VkDeviceCreateInfo& createInfo);
	};
}