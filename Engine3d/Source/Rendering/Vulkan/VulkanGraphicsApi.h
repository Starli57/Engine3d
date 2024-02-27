#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "VulkanContext.h"
#include "GraphicsPipeline.h"
#include "Architecture/Ref.h"
#include "Entities/Level.h"
#include "Models/SwapChainData.h"
#include "Models/DepthBufferModel.h"
#include "Rendering/IGraphicsApi.h"
#include "Rendering/Model/Mesh.h"
#include "Rendering/Vulkan/Mesh/MeshVulkan.h"
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

		Ref<VulkanContext> context;

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

		void RecreateSwapChain();

		//todo: replace
		void CreateUniformBuffers();
		void DisposeUniformBuffers();
		void UpdateUniformBuffer(uint32_t imageIndex);
	};
}