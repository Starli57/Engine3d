#include "EngineCore/Pch.h"

#include <functional>

#include "GraphicsApiVulkan.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Profiler/Profiler.h"
#include "ApiWrappers/FormatUtility.h"
#include "ApiWrappers/VkImageWrapper.h"
#include "ApiWrappers/VkInstanceWrapper.h"
#include "ApiWrappers/VkDeviceWrapper.h"
#include "ApiWrappers/SyncObjectsUtility.h"
#include "ApiWrappers/VkSamplerWrapper.h"
#include "ApiWrappers/VkSurfaceKHRWrapper.h"

namespace VulkanApi
{
	GraphicsApiVulkan::GraphicsApiVulkan(const Ref<Ecs>& ecs, Ref<Engine::InputManager> inputManager, const Ref<Engine::ResourcesStorageVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* glfwWindow)
	{
		this->vulkanContext = new VulkanContext();
		this->ecs = ecs;
		this->inputManager = inputManager;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->vulkanContext->window = glfwWindow;
		this->vulkanContext->pipelinesCollection = CreateRef<PipelinesCollection>(projectSettings);
		this->rollback = CreateRef<Engine::Rollback>("GraphicsApiVulkan");
	}

	GraphicsApiVulkan::~GraphicsApiVulkan()
	{
		rollback.reset();
		delete vulkanContext;
	}

	void GraphicsApiVulkan::Init()
	{
		try
		{
			CreateInstance();
			CreateWindowSurface();
			SelectPhysicalRenderingDevice();
			CreateLogicalDevice();
			CreateTextureSampler();
			CreateSwapChain();
			CreateDescriptorManager();
			CreateRenderPasses();
			CreateDepthBuffer();
			CreateCommandsManager();
			CreateSyncObjects();
		}
		catch (const std::exception& e)
		{
			spdlog::critical("GraphicsApiVulkan critical error: {0}", e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void GraphicsApiVulkan::RecreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(vulkanContext->window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(vulkanContext->window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("Recreate swapchain");
		FinalizeRenderOperations();

		onRenderPassesDispose();
		swapChain->Recreate();
		onRenderPassesCreate();
	}

	void GraphicsApiVulkan::Render()
	{
		const auto acquireStatus = AcquireNextImage();
		if (acquireStatus != VK_SUCCESS) return;

		auto commandBuffer = commandsManager->GetCommandBuffer(frame);
		vkResetFences(vulkanContext->logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(commandBuffer, 0);

		onClientRender();
		
		const VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		const std::array submitCommandBuffers =
		{
			commandBuffer
		};

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frame];
		submitInfo.pSignalSemaphores = &renderFinishedSemaphores[frame];
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		Engine::Profiler::GetInstance().BeginSample("Renderer Submit");
		auto submitStatus = vkQueueSubmit(vulkanContext->graphicsQueue, 1, &submitInfo, drawFences[frame]);
		Engine::Profiler::GetInstance().EndSample();

		Engine::CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChainData->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		Engine::Profiler::GetInstance().BeginSample("vkQueuePresentKHR");
		auto presentStatus = vkQueuePresentKHR(vulkanContext->presentationQueue, &presentInfo);
		Engine::Profiler::GetInstance().EndSample();

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
			return;
		}

		Engine::CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % vulkanContext->maxFramesInFlight;
	}

	VkResult GraphicsApiVulkan::AcquireNextImage()
	{
		vkWaitForFences(vulkanContext->logicalDevice, 1, &drawFences[frame], VK_TRUE, vulkanContext->frameSyncTimeout);

		auto acquireStatus = vkAcquireNextImageKHR(
			vulkanContext->logicalDevice, swapChainData->swapChain, vulkanContext->frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
		}

		return acquireStatus;
	}
	
	void GraphicsApiVulkan::FinalizeRenderOperations()
	{
		vkDeviceWaitIdle(vulkanContext->logicalDevice);
	}

	void GraphicsApiVulkan::BindClientFunctions(std::function<void()> onClientRender, std::function<void()> onRenderPassesCreate, std::function<void()> onRenderPassesDispose)
	{
		this->onClientRender = onClientRender;
		this->onRenderPassesCreate = onRenderPassesCreate;
		this->onRenderPassesDispose = onRenderPassesDispose;
	}

	void GraphicsApiVulkan::CreateInstance() const
	{
		VulkanApi::CreateInstance(vulkanContext->instance);
		rollback->Add([this]() { DisposeInstance(vulkanContext->instance); });
	}

	void GraphicsApiVulkan::CreateWindowSurface() const
	{
		CreateSurface(vulkanContext);
		rollback->Add([this]() { DisposeSurface(vulkanContext); });
	}

	void GraphicsApiVulkan::SelectPhysicalRenderingDevice() const
	{
		ChooseRenderingDevice(vulkanContext);
		vulkanContext->msaa = GetMaxUsableSampleCount(vulkanContext->physicalDevice);
		vulkanContext->depthFormat = FindDepthBufferFormat(vulkanContext->physicalDevice);
		PrintPhysicalDeviceDebugInformation(vulkanContext->physicalDevice, vulkanContext->windowSurface);
	}

	void GraphicsApiVulkan::CreateLogicalDevice() const
	{
		vulkanContext->logicalDevice = VulkanApi::CreateLogicalDevice(vulkanContext);
		rollback->Add([this]()
		{
			DisposeLogicalDevice(vulkanContext->logicalDevice);
		});
	}

	void GraphicsApiVulkan::CreateSwapChain()
	{
		swapChainData = CreateRef<SwapChainData>();
		swapChain = CreateRef<SwapChain>(vulkanContext, swapChainData);

		swapChain->CreateSwapchain();
		swapChain->CreateSwapChainImageViews();
		swapChain->CreateMSAAColorResources();
		swapChain->CreateMSAADepthResources();

		rollback->Add([this] { swapChain->Dispose(); });
	}

	void GraphicsApiVulkan::CreateDescriptorManager()
	{
		descriptorsManager = CreateUniqueRef<DescriptorsManager>(vulkanContext, ecs, inputManager, textureSampler, assetDatabase);
		rollback->Add([this] { descriptorsManager.reset(); });
	}

	void GraphicsApiVulkan::CreateRenderPasses() const
	{
		onRenderPassesCreate();
		rollback->Add([this] { onRenderPassesDispose(); });
	}

	void GraphicsApiVulkan::CreateCommandsManager()
	{
		commandsManager = new CommandsManager(vulkanContext, vulkanContext->maxFramesInFlight);
		rollback->Add([this]() { delete commandsManager; });
	}

	void GraphicsApiVulkan::CreateDepthBuffer() const
	{
		spdlog::info("Create depth buffer");
		swapChain->CreateDepthBuffer();
	}

	void GraphicsApiVulkan::CreateTextureSampler()
	{
		VulkanApi::CreateTextureSampler(vulkanContext->physicalDevice, vulkanContext->logicalDevice, textureSampler);
		rollback->Add([this]() { vkDestroySampler(vulkanContext->logicalDevice, textureSampler, nullptr); });
	}

	void GraphicsApiVulkan::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(vulkanContext->maxFramesInFlight);
		renderFinishedSemaphores.resize(vulkanContext->maxFramesInFlight);
		drawFences.resize(vulkanContext->maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < vulkanContext->maxFramesInFlight; i++)
		{
			CreateVkSemaphore(vulkanContext->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i], rollback);
			CreateVkSemaphore(vulkanContext->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i], rollback);
			CreateVkFence(vulkanContext->logicalDevice, &fenceInfo, nullptr, &drawFences[i], rollback);
		}
	}
}
