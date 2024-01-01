#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "ValidationLayers.h"
#include "PhysicalDeviceSelector.h"
#include "LogicalDeviceCreator.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

private:
	VkInstance instance;
	ValidationLayers* validationLayers;
	PhysicalDeviceSelector* physycalDeviceSelector;
	LogicalDeviceCreator* logicaDeviceCreator;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;
	VkSurfaceKHR windiwSurface;

	void CreateInstance();
	void DestroyInstance();

	void SelectPhysicalRenderingDevice();
	void CreateLogicalDevice();

	std::vector<const char*> GetGLFWRequiredExtensions();
};

