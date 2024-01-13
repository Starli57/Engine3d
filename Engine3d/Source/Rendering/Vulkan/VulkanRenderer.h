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
#include "GraphicsPipeline/AGraphicsPipeline.h"

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

		AGraphicsPipeline* graphicsPipelineInterface;

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

		void CreateRenderPass();
		void DisposeRenderPass();

		void CreateGraphicsPipeline();
		void DisposeGraphicsPipeline();
	};
}
