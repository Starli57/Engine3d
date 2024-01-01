#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "ValidationLayers.h"
#include "PhysicalDeviceSelector.h"
#include "LogicalDeviceCreator.h"
#include "WindowsSurface.h"

class VulkanRenderer
{
public:
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

private:
	GLFWwindow* window;

	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;
	VkSurfaceKHR windowSurface;
	
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	ValidationLayers* validationLayers;
	PhysicalDeviceSelector* physycalDeviceSelector;
	LogicalDeviceCreator* logicaDeviceCreator;
	WindowsSurface* windowsSurfaceCreator;


	void CreateInstance();
	void DestroyInstance();

	void SetupAppInfo(VkApplicationInfo& info);
	void SetupInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkApplicationInfo& appInfo);
	void SelectPhysicalRenderingDevice();
	void CreateLogicalDevice();
	void CreateWindowSurface();

	std::vector<const char*> GetGLFWRequiredExtensions();
};

