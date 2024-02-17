#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "Context/GraphicsPipeline.h"
#include "Entities/World.h"
#include "Model/SwapChainData.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Model/Mesh.h"
#include "Rendering/Vulkan/Mesh/MeshVulkan.h"
#include "Builders/AValidationLayers.h"
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
#include "Components/Transform.h"
#include "Components/MeshContainer.h"
#include "Components/Camera.h"

namespace AVulkan
{

	class VulkanRenderer : public IRenderer
	{
	public:
		VulkanRenderer(entt::registry* ecs, GLFWwindow* window, Rollback* vulkanRollback);
		virtual ~VulkanRenderer() override;

		void Init() override;
		void Render() override;
		void FinanilizeRenderOperations() override;

		void AddMesh(Ref<Mesh> mesh) override;
		void CleanMeshes() override;

		void OnFramebufferResized() override;

	private:
		entt::registry* ecs;
		Rollback* rollback;
		GLFWwindow* window;
		Device* device;

		VkInstance instance;
		VkSurfaceKHR windowSurface;
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
		uint16_t const maxFramesDraws = 3;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		bool needResizeWindow = false;

		void CreateInstance();
		void CreateDevice();
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
		void UpdateUniformBuffers(uint32_t imageIndex);
	};
}
