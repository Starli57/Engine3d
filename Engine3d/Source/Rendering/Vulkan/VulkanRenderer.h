#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "ValidationLayersInterface.h"
#include "PhysicalDeviceInterface.h"
#include "LogicalDeviceInterface.h"
#include "WindowsSurfaceInterface.h"

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

	ValidationLayersInterface* validationLayersInterface;
	PhysicalDeviceInterface* physycalDevicesInterface;
	LogicalDeviceInterface* logicaDeviceInterface;
	WindowsSurfaceInterface* windowsSurfaceInterface;


	void CreateInstance();
	void DestroyInstance();

	void SelectPhysicalRenderingDevice();
	void CreateLogicalDevice();
	void CreateWindowSurface();

	std::vector<const char*> GetGLFWRequiredExtensions();
};

