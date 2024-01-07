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
#include "ShaderModuleInterface.h"

#include "Architecture/Rollback/Rollback.h"

class VulkanRenderer
{
public:
	VulkanRenderer(GLFWwindow* window, Rollback* vulkanRollback);
	~VulkanRenderer();

	void Initialize();

private:
	Rollback* rollback;

	GLFWwindow* window;

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkSurfaceKHR windowSurface;
	SwapChainData swapChainData;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

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

	void CreateGraphicsPipeline();

	void CreateShadersModules();
	void DisposeShadersModules();
};

