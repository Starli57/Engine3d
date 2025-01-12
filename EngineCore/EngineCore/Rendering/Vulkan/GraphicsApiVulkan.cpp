#include "EngineCore/Pch.h"

#include <functional>

#include "GraphicsApiVulkan.h"

namespace AVulkan
{
	GraphicsApiVulkan::GraphicsApiVulkan(Ref<Ecs> ecs, Ref<AssetsDatabaseVulkan> assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* glfwWindow)
	{
		this->ecs = ecs;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->window = glfwWindow;
		this->descriptors = CreateRef<Descriptors>();
		this->pipelinesCollection = CreateRef<PipelinesCollection>(projectSettings);
		this->rendererConfig = CreateRef<VulkanConfiguration>();
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
			CreateRenderPasses();
			CreateDepthBuffer();
			CreateCommandPool();
			CreateCommandBuffer();
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
		glfwGetFramebufferSize(window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("Recreate swapchain");
		FinanilizeRenderOperations();

		delete renderPassColor;
		delete renderPassShadowMaps;

		swapChain->Recreate();

		renderPassShadowMaps = new RenderPassShadowMaps(physicalDevice, logicalDevice,
			rendererConfig, ecs, assetDatabase, swapChainData, descriptors);
		renderPassColor = new RenderPassColor(physicalDevice, logicalDevice,
			rendererConfig, ecs, assetDatabase, swapChainData, descriptors,
			textureSampler, pipelinesCollection, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
	}

	void GraphicsApiVulkan::Render()
	{
		auto commandBuffer = commandBuffers[frame];
		auto acquireStatus = AcquireNextImage();
		if (acquireStatus != VK_SUCCESS) return;

		vkResetFences(logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(commandBuffer, 0);

		VkUtils::BeginCommandBuffer(commandBuffer);
		renderPassShadowMaps->Render(commandBuffer, frame, imageIndex);

		VkUtils::TransitionImageLayout(logicalDevice, commandBuffer, graphicsQueue, renderPassShadowMaps->GetImageBuffer()->image,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

		renderPassColor->Render(commandBuffer, frame, imageIndex);

		VkUtils::TransitionImageLayout(logicalDevice, commandBuffer, graphicsQueue, swapChainData->images.at(imageIndex),
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT);

		VkUtils::EndCommandBuffer(commandBuffer);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		std::array<VkCommandBuffer, 1> submitCommandBuffers =
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
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());;
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		auto submitStatus = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[frame]);
		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChainData->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		auto presentStatus = vkQueuePresentKHR(presentationQueue, &presentInfo);

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % rendererConfig->maxFramesInFlight;
		vkDeviceWaitIdle(logicalDevice);
	}

	VkResult GraphicsApiVulkan::AcquireNextImage()
	{
		vkWaitForFences(logicalDevice, 1, &drawFences[frame], VK_TRUE, rendererConfig->frameSyncTimeout);

		auto acquireStatus = vkAcquireNextImageKHR(
			logicalDevice, swapChainData->swapChain, rendererConfig->frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
		}

		return acquireStatus;
	}
	
	void GraphicsApiVulkan::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	void GraphicsApiVulkan::CreateInstance()
	{
		VkUtils::CreateInstance(instance);
		rollback->Add([this]() { VkUtils::DisposeInstance(instance); });
	}

	void GraphicsApiVulkan::CreateWindowSurface()
	{
		windowSurface = VkUtils::CreateSurface(instance, *window);
		rollback->Add([this]() { VkUtils::DisposeSurface(instance, windowSurface); });
	}

	void GraphicsApiVulkan::SelectPhysicalRenderingDevice()
	{
		physicalDevice = VkUtils::GetBestRenderingDevice(instance, windowSurface);
		rendererConfig->msaa = VkUtils::GetMaxUsableSampleCount(physicalDevice);
		rendererConfig->depthFormat = VkUtils::FindDepthBufferFormat(physicalDevice);
		VkUtils::PrintPhysicalDeviceDebugInformation(physicalDevice, windowSurface);
	}

	void GraphicsApiVulkan::CreateLogicalDevice()
	{
		logicalDevice = VkUtils::CreateLogicalDevice(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
		rollback->Add([this]()
		{
			descriptors->DestroyDescriptorPools(logicalDevice);
			VkUtils::DisposeLogicalDevice(logicalDevice);
		});
	}

	void GraphicsApiVulkan::CreateSwapChain()
	{
		swapChainData = CreateRef<SwapChainData>();
		swapChain = CreateRef<SwapChain>(*window, physicalDevice, logicalDevice, windowSurface, graphicsQueue, swapChainData, rendererConfig);

		swapChain->CreateSwapchain();
		swapChain->CreateSwapChainImageViews();
		swapChain->CreateMSAAColorResources();
		swapChain->CreateMSAADepthResources();

		rollback->Add([this] { swapChain->Dispose(); });
	}

	void GraphicsApiVulkan::CreateRenderPasses()
	{
		renderPassShadowMaps = new RenderPassShadowMaps(physicalDevice, logicalDevice, 
			rendererConfig, ecs, assetDatabase, swapChainData, descriptors);
		renderPassColor = new RenderPassColor(physicalDevice, logicalDevice, 
			rendererConfig, ecs, assetDatabase, swapChainData, descriptors, 
			textureSampler, pipelinesCollection, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());

		rollback->Add([this] { delete renderPassColor; });
		rollback->Add([this] { delete renderPassShadowMaps; });
	}

	void GraphicsApiVulkan::CreateCommandPool()
	{
		commandPool = VkUtils::CreateCommandPool(logicalDevice, physicalDevice, windowSurface);
		rollback->Add([this]() { VkUtils::DisposeCommandPool(logicalDevice, commandPool); });
	}

	void GraphicsApiVulkan::CreateCommandBuffer()
	{
		VkUtils::AllocateCommandBuffers(logicalDevice, commandPool, commandBuffers, rendererConfig->maxFramesInFlight);
		rollback->Add([this]() { VkUtils::FreeCommandBuffers(logicalDevice, commandPool, commandBuffers); });
	}

	void GraphicsApiVulkan::CreateDepthBuffer()
	{
		spdlog::info("Create depth buffer");
		swapChain->CreateDepthBuffer();
	}

	void GraphicsApiVulkan::CreateTextureSampler()
	{
		VkUtils::CreateTextureSampler(physicalDevice, logicalDevice, textureSampler);
		rollback->Add([this]() { vkDestroySampler(logicalDevice, textureSampler, nullptr); });
	}

	void GraphicsApiVulkan::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(rendererConfig->maxFramesInFlight);
		renderFinishedSemaphores.resize(rendererConfig->maxFramesInFlight);
		drawFences.resize(rendererConfig->maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < rendererConfig->maxFramesInFlight; i++)
		{
			VkUtils::CreateVkSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i], rollback);
			VkUtils::CreateVkSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i], rollback);
			VkUtils::CreateVkFence(logicalDevice, &fenceInfo, nullptr, &drawFences[i], rollback);
		}
	}
}