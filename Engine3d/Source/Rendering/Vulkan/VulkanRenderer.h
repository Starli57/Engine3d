#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "GraphicsPipeline.h"
#include "Base/Level.h"
#include "Model/SwapChainData.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Model/Mesh.h"
#include "Rendering/Vulkan/Mesh/MeshVulkan.h"
#include "Builders/AValidationLayers.h"
#include "Builders/APhysicalDevice.h"
#include "Builders/ALogicalDevice.h"
#include "Builders/AWindowsSurface.h"
#include "Builders/AInstance.h"
#include "Builders/AImageView.h"
#include "Builders/AShaderModule.h"
#include "Builders/ARenderPass.h"
#include "Builders/AFrameBuffer.h"
#include "Builders/ACommandPool.h"
#include "Builders/ADescriptorLayout.h"
#include "Builders/ADescriptorPool.h"
#include "Builders/ADescriptorSet.h"
#include "Buffers/ACommandBuffer.h"
#include "Architecture/Rollback/Rollback.h"

namespace AVulkan
{
	class VulkanRenderer : public IRenderer
	{
	public:
		VulkanRenderer(GLFWwindow* window, Level* level, Rollback* vulkanRollback);
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
		Level* level;

		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		VkSurfaceKHR windowSurface;
		VkQueue graphicsQueue;
		VkQueue presentationQueue;
		VkRenderPass renderPass;
		VkCommandPool commandPool;
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;

		SwapChainData swapChainData;
		GraphicsPipeline* graphicsPipeline;

		//todo: replace
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		std::vector<MeshVulkan*>* drawMeshes = nullptr;

		//todo: replace
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
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void RecreateSwapChain();

		void CreateLevelMeshes();

		//todo: replace
		void CreateUniformBuffers();
		void DisposeUniformBuffers();
		void UpdateUniformBuffer(uint32_t imageIndex);
	};
}
