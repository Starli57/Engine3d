#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "SwapChainData.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Data/Mesh.h"
#include "Rendering/Vulkan/Data/MeshVulkan.h"
#include "Builders/AValidationLayers.h"
#include "Builders/APhysicalDevice.h"
#include "Builders/ALogicalDevice.h"
#include "Builders/AWindowsSurface.h"
#include "Builders/AInstance.h"
#include "Builders/AImageView.h"
#include "Builders/AShaderModule.h"
#include "Builders/ARenderPass.h"
#include "Builders/AGraphicsPipeline.h"
#include "Builders/AFrameBuffer.h"
#include "Builders/ACommandPool.h"
#include "Builders/ACommandBuffer.h"
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

		void AddMesh(Mesh& mesh) override;
		void CleanMeshes() override;

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

		std::vector<MeshVulkan*>* drawMeshes;

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
