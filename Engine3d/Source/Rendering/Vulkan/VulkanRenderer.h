#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "ValidationLayersInterface.h"
#include "PhysicalDeviceInterface.h"
#include "LogicalDeviceInterface.h"
#include "WindowsSurfaceInterface.h"
#include "ImageViewInterface.h"

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
	VkSwapchainKHR swapChain;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	ValidationLayersInterface* validationLayersInterface;
	PhysicalDeviceInterface* physicalDevicesInterface;
	LogicalDeviceInterface* logicaDeviceInterface;
	WindowsSurfaceInterface* windowsSurfaceInterface;
	SwapChainInterface* swapChainInterface;
	ImageViewInterface* imageViewInterface;

	void CreateInstance();
	void DestroyInstance();

	void SelectPhysicalRenderingDevice();
	void CreateLogicalDevice();
	void CreateWindowSurface();
	void CreateSwapChain();
	void CreateSwapChainImageViews();

	std::vector<const char*> GetGLFWRequiredExtensions();
};

