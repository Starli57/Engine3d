#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "ValidationLayersInterface.h"

class InstanceInterface
{
public:
	void CreateInstance(VkInstance* instance);
	void DestroyInstance(VkInstance* instance);

private:
	std::vector<const char*> GetGLFWRequiredExtensions();
};

