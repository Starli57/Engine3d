#pragma once

#include <GLFW/glfw3.h>

namespace VulkanApi
{
	void SetupValidationLayers(VkInstanceCreateInfo& createInfo);
	void SetupValidationLayers(VkDeviceCreateInfo& createInfo);;
}