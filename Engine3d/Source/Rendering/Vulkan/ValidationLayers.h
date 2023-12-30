#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class ValidationLayers
{
public:
	void SetupValidationLayers(VkInstanceCreateInfo& createInfo);
};

