#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "ValidationLayers.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

private:
	VkInstance instance;
	ValidationLayers* validationLayers;

	void CreateInstance();
	void DestroyInstance();

	void SetupAppInfo(VkApplicationInfo& info);
	void SetupInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkApplicationInfo& appInfo);

};

