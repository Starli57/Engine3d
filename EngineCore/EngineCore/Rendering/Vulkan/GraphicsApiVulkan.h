#pragma once

#include <GLFW/glfw3.h>

#include <stack>
#include <vector>

#include "CommandsManager.h"
#include "VulkanContext.h"
#include "Descriptors/DescriptorsManager.h"

#include "Models/SwapchainContext.h"

#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

#include "ApiWrappers/VkPhysicalDeviceWrapper.h"

namespace VulkanApi
{
	class GraphicsApiVulkan : public Engine::IGraphicsApi
	{
	public:
		VulkanContext* vulkanContext;

		URef<DescriptorsManager> descriptorsManager;
		Ref<SwapchainManager> swapchainManager;
		SwapchainContext* swapchainContext;

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
		
		GraphicsApiVulkan(const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager, const Ref<Engine::ResourcesStorageVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* window);
		virtual ~GraphicsApiVulkan() override;

		void Init() override;
		void Render() override;
		void FinalizeRenderOperations() override;
		void BindClientFunctions(std::function<void()> onClientRender, std::function<void()> onRenderPassesCreate, std::function<void()> onRenderPassesDispose);

	private:
		Ref<Ecs> ecs;
		Ref<Engine::InputManager> inputManager;
		Ref<Engine::ResourcesStorageVulkan> assetDatabase;
		Ref<ProjectSettings> projectSettings;
		Ref<Engine::Rollback> rollback;
		
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
