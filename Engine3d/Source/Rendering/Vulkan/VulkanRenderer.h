#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "ValidationLayers.h"
#include "PhysicalDeviceSelector.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

private:
	VkInstance instance;
	ValidationLayers* validationLayers;
	PhysicalDeviceSelector* physDeviceSelector;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	void CreateInstance();
	void DestroyInstance();

	void SetupAppInfo(VkApplicationInfo& info);
	void SetupInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkApplicationInfo& appInfo);
	void SelectPhysicalRenderingDevice();

	std::vector<const char*> GetRequiredExtensions();
};

