#include "EngineCore/Pch.h"

#include <functional>

#include "GraphicsApiVulkan.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Profiler/Profiler.h"
#include "Utilities/FormatUtility.h"
#include "Utilities/ImageUtility.h"
#include "Utilities/InstanceUtility.h"
#include "Utilities/LogicalDeviceUtility.h"
#include "Utilities/SyncObjectsUtility.h"
#include "Utilities/TextureSamplerUtility.h"
#include "Utilities/WindowSurfaceUtility.h"

namespace AVulkan
{
	GraphicsApiVulkan::GraphicsApiVulkan(const Ref<Ecs>& ecs, Ref<InputManager> inputManager, const Ref<ResourcesStorageVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* glfwWindow)
	{
		this->context = CreateRef<VulkanContext>();
		this->ecs = ecs;
		this->inputManager = inputManager;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->context->window = glfwWindow;
		this->context->pipelinesCollection = CreateRef<PipelinesCollection>(projectSettings);
		this->rollback = CreateRef<Rollback>("GraphicsApiVulkan");
	}

	GraphicsApiVulkan::~GraphicsApiVulkan()
	{
		rollback.reset();
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
		glfwGetFramebufferSize(context->window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(context->window, &width, &height);
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
		vkResetFences(context->logicalDevice, 1, &drawFences[frame]);
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
		auto submitStatus = vkQueueSubmit(context->graphicsQueue, 1, &submitInfo, drawFences[frame]);
		Profiler::GetInstance().EndSample();
		
		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChainData->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		Profiler::GetInstance().BeginSample("vkQueuePresentKHR");
		auto presentStatus = vkQueuePresentKHR(context->presentationQueue, &presentInfo);
		Profiler::GetInstance().EndSample();

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % context->maxFramesInFlight;
	}

	VkResult GraphicsApiVulkan::AcquireNextImage()
	{
		vkWaitForFences(context->logicalDevice, 1, &drawFences[frame], VK_TRUE, context->frameSyncTimeout);

		auto acquireStatus = vkAcquireNextImageKHR(
			context->logicalDevice, swapChainData->swapChain, context->frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
		}

		return acquireStatus;
	}
	
	void GraphicsApiVulkan::FinalizeRenderOperations()
	{
		vkDeviceWaitIdle(context->logicalDevice);
	}

	void GraphicsApiVulkan::BindClientFunctions(std::function<void()> onClientRender, std::function<void()> onRenderPassesCreate, std::function<void()> onRenderPassesDispose)
	{
		this->onClientRender = onClientRender;
		this->onRenderPassesCreate = onRenderPassesCreate;
		this->onRenderPassesDispose = onRenderPassesDispose;
	}

	void GraphicsApiVulkan::CreateInstance() const
	{
		VkUtils::CreateInstance(context->instance);
		rollback->Add([this]() { VkUtils::DisposeInstance(context->instance); });
	}

	void GraphicsApiVulkan::CreateWindowSurface() const
	{
		context->windowSurface = VkUtils::CreateSurface(context);
		rollback->Add([this]() { VkUtils::DisposeSurface(context); });
	}

	void GraphicsApiVulkan::SelectPhysicalRenderingDevice() const
	{
		context->physicalDevice = VkUtils::GetBestRenderingDevice(context->instance, context->windowSurface);
		context->msaa = VkUtils::GetMaxUsableSampleCount(context->physicalDevice);
		context->depthFormat = VkUtils::FindDepthBufferFormat(context->physicalDevice);
		VkUtils::PrintPhysicalDeviceDebugInformation(context->physicalDevice, context->windowSurface);
	}

	void GraphicsApiVulkan::CreateLogicalDevice() const
	{
		context->logicalDevice = VkUtils::CreateLogicalDevice(context);
		rollback->Add([this]()
		{
			VkUtils::DisposeLogicalDevice(context->logicalDevice);
		});
	}

	void GraphicsApiVulkan::CreateSwapChain()
	{
		swapChainData = CreateRef<SwapChainData>();
		swapChain = CreateRef<SwapChain>(context, swapChainData);

		swapChain->CreateSwapchain();
		swapChain->CreateSwapChainImageViews();
		swapChain->CreateMSAAColorResources();
		swapChain->CreateMSAADepthResources();

		rollback->Add([this] { swapChain->Dispose(); });
	}

	void GraphicsApiVulkan::CreateDescriptorManager()
	{
		descriptorsManager = CreateUniqueRef<DescriptorsManager>(context->physicalDevice, context->logicalDevice, ecs, inputManager,
			textureSampler, assetDatabase);

		rollback->Add([this] { descriptorsManager.reset(); });
	}

	void GraphicsApiVulkan::CreateRenderPasses() const
	{
		onRenderPassesCreate();
		rollback->Add([this] { onRenderPassesDispose(); });
	}

	void GraphicsApiVulkan::CreateCommandsManager()
	{
		commandsManager = new CommandsManager(context, context->maxFramesInFlight);
		rollback->Add([this]() { delete commandsManager; });
	}

	void GraphicsApiVulkan::CreateDepthBuffer() const
	{
		spdlog::info("Create depth buffer");
		swapChain->CreateDepthBuffer();
	}

	void GraphicsApiVulkan::CreateTextureSampler()
	{
		VkUtils::CreateTextureSampler(context->physicalDevice, context->logicalDevice, textureSampler);
		rollback->Add([this]() { vkDestroySampler(context->logicalDevice, textureSampler, nullptr); });
	}

	void GraphicsApiVulkan::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(context->maxFramesInFlight);
		renderFinishedSemaphores.resize(context->maxFramesInFlight);
		drawFences.resize(context->maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < context->maxFramesInFlight; i++)
		{
			VkUtils::CreateVkSemaphore(context->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i], rollback);
			VkUtils::CreateVkSemaphore(context->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i], rollback);
			VkUtils::CreateVkFence(context->logicalDevice, &fenceInfo, nullptr, &drawFences[i], rollback);
		}
	}
}
