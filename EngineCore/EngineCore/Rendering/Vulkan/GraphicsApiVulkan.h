#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "CommandsManager.h"
#include "VulkanContext.h"
#include "Descriptors/DescriptorsManager.h"

#include "Models/SwapChainData.h"

#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

#include "Utilities/PhysicalDeviceUtility.h"

namespace AVulkan
{
	class GraphicsApiVulkan : public IGraphicsApi
	{
	public:
		Ref<VulkanContext> context;

		URef<DescriptorsManager> descriptorsManager;
		Ref<SwapChain> swapChain;
		Ref<SwapChainData> swapChainData;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		VkSampler textureSampler;

		std::function<void()> onClientRender;
		std::function<void()> onRenderPassesCreate;
		std::function<void()> onRenderPassesDispose;
		
		uint32_t GetImageIndex() const { return imageIndex; }
		uint16_t GetFrame() const { return frame; }
		
		VkCommandPool& GetCommandPool() const { return commandsManager->GetCommandPool(); }
		VkCommandBuffer& GetCommandBuffer() const { return commandsManager->GetCommandBuffer(frame); }
		
		GraphicsApiVulkan(const Ref<Ecs>& ecs, Ref<InputManager> inputManager, const Ref<ResourcesStorageVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* window);
		virtual ~GraphicsApiVulkan() override;

		void Init() override;
		void Render() override;
		void FinalizeRenderOperations() override;
		void BindClientFunctions(std::function<void()> onClientRender, std::function<void()> onRenderPassesCreate, std::function<void()> onRenderPassesDispose);

	private:
		Ref<Ecs> ecs;
		Ref<InputManager> inputManager;
		Ref<ResourcesStorageVulkan> assetDatabase;
		Ref<ProjectSettings> projectSettings;
		Ref<Rollback> rollback;
		
		CommandsManager* commandsManager;

		uint32_t imageIndex = 0;
		uint16_t frame = 0;

		void CreateInstance() const;
		void SelectPhysicalRenderingDevice() const;
		void CreateLogicalDevice() const;
		void CreateWindowSurface() const;
		void CreateSwapChain();
		void CreateDescriptorManager();
		void CreateRenderPasses() const;
		void CreateDepthBuffer() const;
		void CreateCommandsManager();
		void CreateSyncObjects();
		void CreateTextureSampler();

		void RecreateSwapChain();

		VkResult AcquireNextImage();
	};
}
