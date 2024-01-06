#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <functional>

#include "ValidationLayersInterface.h"
#include "PhysicalDeviceInterface.h"
#include "LogicalDeviceInterface.h"
#include "WindowsSurfaceInterface.h"
#include "ImageViewInterface.h"
#include "InstanceInterface.h"
#include "SwapChainData.h"

class VulkanRenderer
{
public:
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

private:
	GLFWwindow* window;

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkSurfaceKHR windowSurface;
	SwapChainData swapChainData;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	std::stack<std::function<void()>> disposeStack;

	void CreateInstance();
	void DisposeInstance();

	void SelectPhysicalRenderingDevice();

	void CreateLogicalDevice();
	void DisposeLogicalDevice();

	void CreateWindowSurface();
	void DisposeSurface();

	void CreateSwapChain();
	void DisposeSwapChain();

	void CreateSwapChainImageViews();
	void DisposeSwapChainImageViews();

	void DisposeAquiredVulkanResources();
};

