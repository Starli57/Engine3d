#include "EngineCore/Pch.h"

#include <functional>

#include "GraphicsApiVulkan.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Profiler/Profiler.h"
#include "ApiWrappers/VkImageWrapper.h"
#include "ApiWrappers/VkInstanceWrapper.h"
#include "ApiWrappers/VkDeviceWrapper.h"
#include "ApiWrappers/SyncObjectsUtility.h"
#include "ApiWrappers/VkSamplerWrapper.h"
#include "ApiWrappers/VkSurfaceKHRWrapper.h"

namespace Engine
{
	GraphicsApiVulkan::GraphicsApiVulkan(const Ref<Ecs>& ecs, Ref<InputManager> inputManager,
		const Ref<ResourcesStorageVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* glfwWindow)
	{
		this->vulkanContext = new VulkanApi::VulkanContext();
		this->ecs = ecs;
		this->inputManager = inputManager;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->vulkanContext->window = glfwWindow;
		this->vulkanContext->pipelinesCollection = CreateRef<PipelinesCollection>(projectSettings);
		this->rollback = CreateRef<Rollback>("GraphicsApiVulkan");
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

	void GraphicsApiVulkan::DisposeAndRecreateSwapchain()
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
		VulkanApi::RecreateSwapchain(vulkanContext);
		onRenderPassesCreate();
	}

	void GraphicsApiVulkan::Render()
	{
		const auto acquireStatus = AcquireNextImage();
		if (acquireStatus != VK_SUCCESS) return;

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		commandsManager->RefCommandBuffer(frame, commandBuffer);

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
		presentInfo.pSwapchains = &swapchainContext->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		Profiler::GetInstance().BeginSample("vkQueuePresentKHR");
		auto presentStatus = vkQueuePresentKHR(vulkanContext->presentationQueue, &presentInfo);
		Profiler::GetInstance().EndSample();

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
		{
			DisposeAndRecreateSwapchain();
			return;
		}

		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % VulkanApi::VulkanContext::maxFramesInFlight;
	}

	VkResult GraphicsApiVulkan::AcquireNextImage()
	{
		vkWaitForFences(vulkanContext->logicalDevice, 1, &drawFences[frame], VK_TRUE, vulkanContext->frameSyncTimeout);

		auto acquireStatus = vkAcquireNextImageKHR(
			vulkanContext->logicalDevice, swapchainContext->swapChain, vulkanContext->frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			DisposeAndRecreateSwapchain();
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
		rollback->Add([this]() { VulkanApi::DisposeInstance(vulkanContext->instance); });
	}

	void GraphicsApiVulkan::CreateWindowSurface() const
	{
		CreateSurface(vulkanContext);
		rollback->Add([this]() { DisposeSurface(vulkanContext); });
	}

	void GraphicsApiVulkan::SelectPhysicalRenderingDevice() const
	{
		ChooseRenderingDevice(vulkanContext);
		SetQueueFamilies(vulkanContext);
		SetMaxUsableSampleCount(vulkanContext);
		SetDepthBufferFormat(vulkanContext);
		PrintPhysicalDeviceDebugInformation(vulkanContext);
	}

	void GraphicsApiVulkan::CreateLogicalDevice() const
	{
		vulkanContext->logicalDevice = VulkanApi::CreateLogicalDevice(vulkanContext);
		rollback->Add([this]()
		{
			VulkanApi::DisposeLogicalDevice(vulkanContext->logicalDevice);
		});
	}

	void GraphicsApiVulkan::CreateSwapChain()
	{
		CreateSwapchain(vulkanContext);
		CreateSwapchainImageViews(vulkanContext);
		CreateMSAAColorResources(vulkanContext);
		CreateMSAADepthResources(vulkanContext);

		rollback->Add([this] { DisposeSwapchain(vulkanContext); });
	}

	void GraphicsApiVulkan::CreateDescriptorManager()
	{
		descriptorsManager = CreateUniqueRef<VulkanApi::DescriptorsManager>(vulkanContext, ecs, inputManager, textureSampler, assetDatabase);
		rollback->Add([this] { descriptorsManager.reset(); });
	}

	void GraphicsApiVulkan::CreateRenderPasses() const
	{
		onRenderPassesCreate();
		rollback->Add([this] { onRenderPassesDispose(); });
	}

	void GraphicsApiVulkan::CreateCommandsManager()
	{
		commandsManager = new VulkanApi::CommandsManager(vulkanContext, VulkanApi::VulkanContext::maxFramesInFlight);
		rollback->Add([this]() { delete commandsManager; });
	}

	void GraphicsApiVulkan::CreateDepthBuffer() const
	{
		spdlog::info("Create depth buffer");
		VulkanApi::CreateDepthBuffer(vulkanContext);
	}

	void GraphicsApiVulkan::CreateTextureSampler()
	{
		VulkanApi::CreateTextureSampler(vulkanContext->physicalDevice, vulkanContext->logicalDevice, textureSampler);
		rollback->Add([this]() { vkDestroySampler(vulkanContext->logicalDevice, textureSampler, nullptr); });
	}

	void GraphicsApiVulkan::CreateSyncObjects()
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
