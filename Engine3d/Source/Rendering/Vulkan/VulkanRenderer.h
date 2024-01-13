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
#include "ARenderPass.h"
#include "AGraphicsPipeline.h"

#include "Architecture/Rollback/Rollback.h"

namespace AVulkan
{
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
		VkRenderPass renderPass;
		VkPipeline graphicsPipeline;

		AGraphicsPipeline* graphicsPipelineInterface;

		void CreateInstance();
		void SelectPhysicalRenderingDevice();
		void CreateLogicalDevice();
		void CreateWindowSurface();
		void CreateSwapChain();
		void CreateSwapChainImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
	};
}
