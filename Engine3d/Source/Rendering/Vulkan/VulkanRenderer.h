#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <functional>

#include "AValidationLayers.h"
#include "APhysicalDevice.h"
#include "ALogicalDevice.h"
#include "AWindowsSurface.h"
#include "AInstance.h"
#include "AImageView.h"
#include "SwapChainData.h"
#include "AShaderModule.h"

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

