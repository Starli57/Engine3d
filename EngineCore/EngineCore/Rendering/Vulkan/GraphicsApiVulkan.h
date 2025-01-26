#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "CommandsManager.h"
#include "DescriptorsManager.h"

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
		Ref<DescriptorsManager> descriptorsManager;
		Ref<VulkanConfiguration> rendererConfig;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		VkSampler textureSampler;

		uint32_t GetImageIndex() const { return imageIndex; }
		uint16_t GetFrame() const { return frame; }
		
		VkCommandPool& GetCommandPool() const { return commandsManager->GetCommandPool(); }
		VkCommandBuffer& GetCommandBuffer() const { return commandsManager->GetCommandBuffer(frame); }
		
		RenderPassColor* GetRenderPassColor() const { return renderPassColor; }
		RenderPassShadowMaps* GetRenderPassShadowMap() const { return renderPassShadowMaps; }

		GraphicsApiVulkan(const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* window);
		virtual ~GraphicsApiVulkan() override;

		void Init() override;
		void Render() override;
		void FinalizeRenderOperations() override;

	private:
		Ref<Ecs> ecs;
		Ref<AssetsDatabaseVulkan> assetDatabase;
		Ref<ProjectSettings> projectSettings;
		Ref<Rollback> rollback;

		Ref<PipelinesCollection> pipelinesCollection;

		CommandsManager* commandsManager;
		
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
		void CreateDepthBuffer() const;
		void CreateCommandsManager();
		void CreateSyncObjects();
		void CreateTextureSampler();

		void RecreateSwapChain();

		VkResult AcquireNextImage();
	};
}
