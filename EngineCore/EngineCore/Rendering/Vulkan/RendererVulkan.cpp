#include "EngineCore/Pch.h"

#include <functional>

#include "RendererVulkan.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Profiler/Profiler.h"
#include "ApiWrappers/VkImageWrapper.h"
#include "ApiWrappers/VkInstanceWrapper.h"
#include "ApiWrappers/VkDeviceWrapper.h"
#include "ApiWrappers/SyncObjectsUtility.h"
#include "ApiWrappers/VkSamplerWrapper.h"
#include "ApiWrappers/VkSurfaceKHRWrapper.h"
#include "ApiWrappers/VkSwapchainWrapper.h"

namespace Engine
{
	RendererVulkan::RendererVulkan(const Ref<Ecs>& ecs, Ref<InputManager> inputManager,
		const Ref<ResourcesStorageVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* glfwWindow)
	{
		this->vulkanContext = new VulkanApi::VulkanContext();
		this->ecs = ecs;
		this->inputManager = inputManager;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->vulkanContext->window = glfwWindow;
		this->vulkanContext->pipelinesCollection = CreateRef<PipelinesCollection>(projectSettings);
		this->rollback = CreateRef<Rollback>("RendererVulkan");
	}

	RendererVulkan::~RendererVulkan()
	{
		rollback.reset();
		delete vulkanContext->swapchainContext;
		delete vulkanContext;
	}

	void RendererVulkan::Init()
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
			spdlog::critical("RendererVulkan critical error: {0}", e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void RendererVulkan::RecreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(vulkanContext->window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(vulkanContext->window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("RecreateSwapchain swapchain");
		FinalizeRenderOperations();

		onRenderPassesDispose();
		VulkanApi::RecreateSwapchain(vulkanContext);
		onRenderPassesCreate();
	}

	void RendererVulkan::Render()
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

		Profiler::GetInstance().BeginSample("Renderer Submit");
		auto submitStatus = vkQueueSubmit(vulkanContext->graphicsQueue, 1, &submitInfo, drawFences[frame]);
		Profiler::GetInstance().EndSample();

		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &vulkanContext->swapchainContext->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		Profiler::GetInstance().BeginSample("vkQueuePresentKHR");
		auto presentStatus = vkQueuePresentKHR(vulkanContext->presentationQueue, &presentInfo);
		Profiler::GetInstance().EndSample();

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
			return;
		}

		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % VulkanApi::VulkanContext::maxFramesInFlight;
	}

	VkResult RendererVulkan::AcquireNextImage()
	{
		vkWaitForFences(vulkanContext->logicalDevice, 1, &drawFences[frame], VK_TRUE, vulkanContext->frameSyncTimeout);

		auto acquireStatus = vkAcquireNextImageKHR(
			vulkanContext->logicalDevice, vulkanContext->swapchainContext->swapChain, vulkanContext->frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
		}

		return acquireStatus;
	}
	
	void RendererVulkan::FinalizeRenderOperations()
	{
		vkDeviceWaitIdle(vulkanContext->logicalDevice);
	}

	void RendererVulkan::BindClientFunctions(std::function<void()> onClientRender, std::function<void()> onRenderPassesCreate, std::function<void()> onRenderPassesDispose)
	{
		this->onClientRender = onClientRender;
		this->onRenderPassesCreate = onRenderPassesCreate;
		this->onRenderPassesDispose = onRenderPassesDispose;
	}

	void RendererVulkan::CreateInstance() const
	{
		VulkanApi::CreateInstance(vulkanContext->instance);
		rollback->Add([this]() { VulkanApi::DisposeInstance(vulkanContext->instance); });
	}

	void RendererVulkan::CreateWindowSurface() const
	{
		CreateSurface(vulkanContext);
		rollback->Add([this]() { DisposeSurface(vulkanContext); });
	}

	void RendererVulkan::SelectPhysicalRenderingDevice() const
	{
		ChooseRenderingDevice(vulkanContext);
		vulkanContext->msaa = VulkanApi::GetMaxUsableSampleCount(vulkanContext->physicalDevice);
		vulkanContext->physicalDeviceQueueIndices = VulkanApi::GetQueueFamilies(vulkanContext->physicalDevice, vulkanContext->windowSurface);
		vulkanContext->depthFormat = VulkanApi::GetDepthBufferFormat(vulkanContext->physicalDevice);
		VulkanApi::PrintPhysicalDeviceDebugInformation(vulkanContext->physicalDevice, vulkanContext->windowSurface);
	}

	void RendererVulkan::CreateLogicalDevice() const
	{
		vulkanContext->logicalDevice = VulkanApi::CreateLogicalDevice(vulkanContext);
		rollback->Add([this]()
		{
			VulkanApi::DisposeLogicalDevice(vulkanContext->logicalDevice);
		});
	}

	void RendererVulkan::CreateSwapChain()
	{
		vulkanContext->swapchainContext = new VulkanApi::SwapchainContext();
		VulkanApi::CreateSwapchain(vulkanContext);
		VulkanApi::CreateSwapChainImageViews(vulkanContext);
		VulkanApi::CreateMSAAColorResources(vulkanContext);
		VulkanApi::CreateMSAADepthResources(vulkanContext);

		rollback->Add([this] { VulkanApi::DisposeSwapchain(vulkanContext); });
	}

	void RendererVulkan::CreateDescriptorManager()
	{
		descriptorsManager = CreateUniqueRef<VulkanApi::DescriptorsManager>(vulkanContext, ecs, inputManager, textureSampler, assetDatabase);
		rollback->Add([this] { descriptorsManager.reset(); });
	}

	void RendererVulkan::CreateRenderPasses() const
	{
		onRenderPassesCreate();
		rollback->Add([this] { onRenderPassesDispose(); });
	}

	void RendererVulkan::CreateCommandsManager()
	{
		commandsManager = new VulkanApi::CommandsManager(vulkanContext, VulkanApi::VulkanContext::maxFramesInFlight);
		rollback->Add([this]() { delete commandsManager; });
	}

	void RendererVulkan::CreateDepthBuffer() const
	{
		spdlog::info("Create depth buffer");
		VulkanApi::CreateDepthBuffer(vulkanContext);
	}

	void RendererVulkan::CreateTextureSampler()
	{
		VulkanApi::CreateTextureSampler(vulkanContext->physicalDevice, vulkanContext->logicalDevice, textureSampler);
		rollback->Add([this]() { vkDestroySampler(vulkanContext->logicalDevice, textureSampler, nullptr); });
	}

	void RendererVulkan::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(VulkanApi::VulkanContext::maxFramesInFlight);
		renderFinishedSemaphores.resize(VulkanApi::VulkanContext::maxFramesInFlight);
		drawFences.resize(VulkanApi::VulkanContext::maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < VulkanApi::VulkanContext::maxFramesInFlight; i++)
		{
			VulkanApi::CreateVkSemaphore(vulkanContext->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i], rollback);
			VulkanApi::CreateVkSemaphore(vulkanContext->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i], rollback);
			VulkanApi::CreateVkFence(vulkanContext->logicalDevice, &fenceInfo, &drawFences[i], rollback);
		}
	}
}
