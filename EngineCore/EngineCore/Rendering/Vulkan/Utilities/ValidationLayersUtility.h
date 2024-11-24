#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	void SetupValidationLayers(VkInstanceCreateInfo& createInfo);
	void SetupValidationLayers(VkDeviceCreateInfo& createInfo);;
}