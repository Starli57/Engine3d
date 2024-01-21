#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "Rendering/IRenderer.h"
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
#include "AFrameBuffer.h"
#include "ACommandPool.h"
#include "ACommandBuffer.h"

#include "Architecture/Rollback/Rollback.h"

namespace AVulkan
{
	class VulkanRenderer : public IRenderer
	{
	public:
		VulkanRenderer(GLFWwindow* window, Rollback* vulkanRollback);
		virtual ~VulkanRenderer() override;

		void Init() override;
		void Render() override;
		void FinanilizeRenderOperations() override;

		void OnFramebufferResized() override;

	private:
		Rollback* rollback;
		GLFWwindow* window;

		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		VkSurfaceKHR windowSurface;
		VkQueue graphicsQueue;
		VkQueue presentationQueue;
		VkRenderPass renderPass;
		VkPipeline graphicsPipeline;
		VkCommandPool commandPool;

		SwapChainData swapChainData;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		uint16_t frame = 0;
		uint16_t const maxFramesDraws = 2;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		bool needResizeWindow = false;

		void CreateInstance();
		void SelectPhysicalRenderingDevice();
		void CreateLogicalDevice();
		void CreateWindowSurface();
		void CreateSwapChain();
		void CreateSwapChainImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFrameBuffers();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void CreateSyncObjects();

		void RecreateSwapChain();
	};
}
