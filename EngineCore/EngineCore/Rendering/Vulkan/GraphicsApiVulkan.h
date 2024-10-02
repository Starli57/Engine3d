#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "Descriptors.h"

#include "Models/SwapChainData.h"
#include "Models/ImageModel.h"

#include "EngineCore/AssetsManagement/AssetsDatabase.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

#include "EngineCore/Rendering/Vulkan/MeshVulkan.h"
#include "EngineCore/Rendering/Vulkan/TextureVulkan.h"
#include "EngineCore/Rendering/Vulkan/MaterialVulkan.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"

#include "Builders/AValidationLayers.h"
#include "Builders/AImage.h"
#include "Builders/AImageView.h"
#include "Builders/AShaderModule.h"
#include "Utilities/CommandPoolUtility.h"
#include "Utilities/CommandBufferUtility.h"
#include "Utilities/FormatUtility.h"
#include "Utilities/GraphicsPipelineUtility.h"
#include "Utilities/PhysicalDeviceUtility.h"
#include "Utilities/LogicalDeviceUtility.h"
#include "Utilities/RenderPassUtility.h"
#include "Utilities/WindowSurfaceUtility.h"
#include "Utilities/InstanceUtility.h"
#include "Utilities/FrameBufferUtility.h"
#include "Utilities/SyncObjectsUtility.h"
#include "Utilities/MemoryUtility.h"

#include "EngineCore/Assets/Mesh.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"

#include "EngineCore/Systems/TransformSystem.h"
#include "EngineCore/Systems/Camera.h"

namespace AVulkan
{
	class GraphicsApiVulkan : public IGraphicsApi
	{
	public:

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
		Ref<VulkanConfiguration> rendererConfig;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		VkSampler textureSampler;

		uint32_t GetImageIndex() { return imageIndex; }
		uint16_t GetFrame() { return frame; }

		GraphicsApiVulkan(Ref<Ecs> ecs, Ref<AssetsDatabase> assetDatabase, Ref<ProjectSettigns> projectSettings, 
			GLFWwindow* window, Rollback* vulkanRollback);
		virtual ~GraphicsApiVulkan() override;

		void Init() override;
		void Render() override;
		void FinanilizeRenderOperations() override;

		Ref<Mesh> LoadMesh(const std::filesystem::path& meshPath) override;
		Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) override;

		Ref<Texture> CreateTexture(const std::filesystem::path& textureFilePath) override;
		Ref<Material> CreateMaterial(const std::string& pipelineId) override;

	private:
		Ref<Ecs> ecs;
		Ref<AssetsDatabase> assetDatabase;
		Ref<ProjectSettigns> projectSettings;
		Ref<Rollback> rollback;

		Ref<PipelinesCollection> pipelinesCollection;
		std::unordered_map<std::string, Ref<PipelineVulkan>> pipelines;

		uint32_t imageIndex = 0;
		uint16_t frame = 0;

		void CreateInstance();
		void SelectPhysicalRenderingDevice();
		void CreateLogicalDevice();
		void CreateWindowSurface();
		void CreateSwapChain();
		void CreateRenderPass();
		void CreateGraphicsPipelines();
		void CreateDepthBuffer();
		void CreateFrameBuffers();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void CreateSyncObjects();
		void CreateDescriptorSetLayout();
		void CreateTextureSampler();

		void RecreateSwapChain();

		void UpdateUniformBuffer(uint32_t frame);

		void DisposePipelines();
	};
}
