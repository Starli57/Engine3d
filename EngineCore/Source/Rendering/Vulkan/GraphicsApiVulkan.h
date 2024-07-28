#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "AssetsDatabase.h"
#include "Descriptors.h"

#include "Entities/Level.h"

#include "Models/SwapChainData.h"
#include "Models/ImageModel.h"

#include "Rendering/IGraphicsApi.h"
#include "Rendering/PipelinesCollection.h"

#include "Rendering/Vulkan/MeshVulkan.h"
#include "Rendering/Vulkan/TextureVulkan.h"
#include "Rendering/Vulkan/MaterialVulkan.h"
#include "Rendering/Vulkan/PipelineVulkan.h"

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

#include "EngineShared/Mesh.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/ProjectSettings.h"
#include "EngineShared/Rollback/Rollback.h"
#include "EngineShared/Components/MaterialComponent.h"
#include "EngineShared/Components/MeshComponent.h"
#include "EngineShared/Components/UboViewProjectionComponent.h"
#include "EngineShared/Components/UboDiffuseLightComponent.h"

#include "Systems/TransformSystem.h"
#include "Systems/Camera.h"

namespace AVulkan
{
	class GraphicsApiVulkan : public IGraphicsApi
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

		Ref<Mesh> CreateMesh(const std::filesystem::path& meshPath) override;
		Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) override;

		Ref<Texture> CreateTexture(std::filesystem::path& textureFilePath) override;
		Ref<Material> CreateMaterial(std::string& pipelineId) override;

	private:
		Ref<Ecs> ecs;
		Ref<AssetsDatabase> assetDatabase;
		Ref<ProjectSettigns> projectSettings;
		Ref<Rollback> rollback;

		Ref<PipelinesCollection> pipelinesCollection;
		std::unordered_map<std::string, Ref<PipelineVulkan>> pipelines;

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
