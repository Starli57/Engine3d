#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include <IEngineEditor.h>
#include "GraphicsPipeline.h"
#include "SharedLib/Ref.h"

#include "Entities/Level.h"
#include "Entities/Mesh.h"

#include "Models/SwapChainData.h"
#include "Models/ImageModel.h"

#include "Rendering/IGraphicsApi.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Rendering/Vulkan/Entities/TextureVulkan.h"

#include "Builders/AValidationLayers.h"
#include "Builders/APhysicalDevice.h"
#include "Builders/ALogicalDevice.h"
#include "Builders/AImage.h"
#include "Builders/AImageView.h"
#include "Builders/AShaderModule.h"
#include "Descriptors.h"

#include "Utilities/RenderPassUtility.h"
#include "Utilities/WindowSurfaceUtility.h"
#include "Utilities/InstanceUtility.h"
#include "Utilities/FrameBufferUtility.h"
#include "Utilities/CommandPoolUtility.h"
#include "Utilities/CommandBufferUtility.h"

#include "SharedLib/ProjectSettings.h"
#include "SharedLib/Rollback/Rollback.h"
#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "Systems/TransformSystem.h"
#include "Systems/Camera.h"

namespace AVulkan
{
	class VulkanGraphicsApi : public IGraphicsApi
	{
	public:
		static constexpr uint16_t maxFramesInFlight = 2;

		GLFWwindow* window;

		VkInstance instance;
		VkSurfaceKHR windowSurface;

		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkQueue graphicsQueue;
		VkQueue presentationQueue;

		VkRenderPass renderPass;

		Ref<SwapChain> swapChain;
		Ref<SwapChainData> swapChainData;
		Ref<Descriptors> descriptors;

		GraphicsPipeline* graphicsPipeline;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		VkSampler textureSampler;

		uint32_t GetImageIndex() { return imageIndex; }
		uint16_t GetFrame() { return frame; }

		VulkanGraphicsApi(Ref<entt::registry> ecs, Ref<ProjectSettigns> projectSettings, GLFWwindow* window, Rollback* vulkanRollback);
		virtual ~VulkanGraphicsApi() override;

		void Init() override;
		void Render() override;
		void FinanilizeRenderOperations() override;

		Ref<Mesh> CreateMesh(const std::string& meshPath) override;
		Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) override;
		Ref<Texture> CreateTexture(TextureId textureIdh) override;



	private:
		Ref<entt::registry> ecs;
		Ref<ProjectSettigns> projectSettings;
		Ref<Rollback> rollback;
		Ref<IEngineEditor> editor;

		uint32_t imageIndex = 0;
		uint16_t frame = 0;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		void CreateInstance();
		void SelectPhysicalRenderingDevice();
		void CreateLogicalDevice();
		void CreateWindowSurface();
		void CreateSwapChain();
		void CreateSwapChainImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateDepthBuffer();
		void CreateFrameBuffers();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void CreateSyncObjects();
		void CreateDescriptorSetLayout();
		void CreateTextureSampler();

		void RecreateSwapChain();

		void UpdateUniformBuffer(uint32_t frame);
	};
}
