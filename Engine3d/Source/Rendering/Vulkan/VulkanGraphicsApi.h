#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "GraphicsPipeline.h"
#include "Architecture/Ref.h"
#include "Entities/Level.h"
#include "Models/SwapChainData.h"
#include "Models/DepthBufferModel.h"
#include "Rendering/IGraphicsApi.h"
#include "Rendering/Entity/Mesh.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Builders/AValidationLayers.h"
#include "Builders/APhysicalDevice.h"
#include "Builders/ALogicalDevice.h"
#include "Builders/AWindowsSurface.h"
#include "Builders/AInstance.h"
#include "Builders/AImage.h"
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
#include "Components/Transform.h"
#include "Components/MeshContainer.h"
#include "Components/Camera.h"

namespace AVulkan
{
	class VulkanGraphicsApi : public IGraphicsApi
	{
	public:
		VulkanGraphicsApi(Ref<entt::registry> ecs, GLFWwindow* window, Rollback* vulkanRollback);
		virtual ~VulkanGraphicsApi() override;

		void Init() override;
		void Render() override;
		void FinanilizeRenderOperations() override;

		Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) override;

		void OnFramebufferResized() override;

	private:
		Ref<entt::registry> ecs;
		GLFWwindow* window;

		Rollback* rollback;
		Ref<Rollback> swapchainRollback;

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
		Ref<DepthBufferModel> depthBufferModel;
		GraphicsPipeline* graphicsPipeline;

		//todo: replace
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		//todo: replace
		uint16_t frame = 0;
		uint16_t const maxFramesDraws = 2;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		//todo: replace
		VkSampler textureSampler;

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
		void CreateDepthBuffer();
		void CreateTextureSampler();

		void RecreateSwapChain();

		//todo: replace
		void CreateUniformBuffers();
		void DisposeUniformBuffers();
		void UpdateUniformBuffer(uint32_t imageIndex);
	};
}
