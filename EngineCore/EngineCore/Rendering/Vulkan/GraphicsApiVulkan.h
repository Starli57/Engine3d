#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "Descriptors.h"

#include "Models/SwapChainData.h"
#include "Models/ImageModel.h"

#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"

#include "RenderPasses/RenderPassColor.h"
#include "RenderPasses/RenderPassShadowMaps.h"

#include "Utilities/ValidationLayersUtility.h"
#include "Utilities/ImageUtility.h"
#include "Utilities/ShaderModuleUtility.h"
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
#include "Utilities/TextureSamplerUtility.h"

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
#include "EngineCore/Systems/ViewProjectionSystem.h"

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

		RenderPassColor* GetRenderPassColor() const { return renderPassColor; }
		RenderPassShadowMaps* GetRenderPassShadowMap() const { return renderPassShadowMaps; }

		GraphicsApiVulkan(Ref<Ecs> ecs, Ref<AssetsDatabaseVulkan> assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* window);
		virtual ~GraphicsApiVulkan() override;

		void Init() override;
		void Render() override;
		void FinanilizeRenderOperations() override;

	private:
		Ref<Ecs> ecs;
		Ref<AssetsDatabaseVulkan> assetDatabase;
		Ref<ProjectSettings> projectSettings;
		Ref<Rollback> rollback;

		Ref<PipelinesCollection> pipelinesCollection;

		RenderPassColor* renderPassColor;
		RenderPassShadowMaps* renderPassShadowMaps;
		
		uint32_t imageIndex = 0;
		uint16_t frame = 0;

		void CreateInstance();
		void SelectPhysicalRenderingDevice();
		void CreateLogicalDevice();
		void CreateWindowSurface();
		void CreateSwapChain();
		void CreateRenderPasses();
		void CreateDepthBuffer();
		void CreateCommandPool();
		void CreateCommandBuffer();
		void CreateSyncObjects();
		void CreateTextureSampler();

		void RecreateSwapChain();

		VkResult AcquireNextImage();
	};
}
