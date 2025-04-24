#include "EngineCore/Pch.h"

#include <functional>

#include "GraphicsApiVulkan.h"

#include "EngineCore/Profiler/Profiler.h"

namespace AVulkan
{
	GraphicsApiVulkan::GraphicsApiVulkan(const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetDatabase, Ref<ProjectSettings> projectSettings, GLFWwindow* glfwWindow)
	{
		this->ecs = ecs;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->window = glfwWindow;
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
		glfwGetFramebufferSize(window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("Recreate swapchain");
		FinalizeRenderOperations();

		delete renderPassColor;
		delete renderPassShadowMaps;

		swapChain->Recreate();

		renderPassShadowMaps = new RenderPassShadowMaps(physicalDevice, logicalDevice,
			rendererConfig, descriptorsManager, ecs, assetDatabase, swapChainData, pipelinesCollection);
		renderPassColor = new RenderPassColor(physicalDevice, logicalDevice,
			rendererConfig, descriptorsManager, ecs, assetDatabase, swapChainData,
			textureSampler, pipelinesCollection, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
	}

	void GraphicsApiVulkan::Render()
	{
		const auto acquireStatus = AcquireNextImage();
		if (acquireStatus != VK_SUCCESS) return;

		auto commandBuffer = commandsManager->GetCommandBuffer(frame);
		vkResetFences(logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(commandBuffer, 0);

		descriptorsManager->UpdateFrameDescriptors(frame);
		descriptorsManager->UpdateMaterialsDescriptors(frame);
		
		VkUtils::BeginCommandBuffer(commandBuffer);
		renderPassShadowMaps->Render(commandBuffer, frame, imageIndex);

		VkUtils::TransitionImageLayout(commandBuffer, renderPassShadowMaps->GetImageBuffer()->image,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

		descriptorsManager->UpdateShadowsMapDescriptors(frame, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
		
		renderPassColor->Render(commandBuffer, frame, imageIndex);

		VkUtils::TransitionImageLayout(commandBuffer, swapChainData->images.at(imageIndex),
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT);

		VkUtils::EndCommandBuffer(commandBuffer);

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
		auto submitStatus = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[frame]);
		Profiler::GetInstance().EndSample("Renderer Submit");
		
		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChainData->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		Profiler::GetInstance().BeginSample("Renderer Present");
		auto presentStatus = vkQueuePresentKHR(presentationQueue, &presentInfo);
		Profiler::GetInstance().EndSample("Renderer Present");

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % rendererConfig->maxFramesInFlight;
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
	
	void GraphicsApiVulkan::FinalizeRenderOperations()
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

	void GraphicsApiVulkan::CreateDescriptorManager()
	{
		descriptorsManager = CreateRef<DescriptorsManager>(physicalDevice, logicalDevice, ecs,
			textureSampler, assetDatabase);

		rollback->Add([this] { descriptorsManager.reset(); });
	}

	void GraphicsApiVulkan::CreateRenderPasses()
	{
		renderPassShadowMaps = new RenderPassShadowMaps(physicalDevice, logicalDevice, 
			rendererConfig, descriptorsManager, ecs, assetDatabase, swapChainData, pipelinesCollection);
		renderPassColor = new RenderPassColor(physicalDevice, logicalDevice, 
			rendererConfig, descriptorsManager, ecs, assetDatabase, swapChainData, 
			textureSampler, pipelinesCollection, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());

		rollback->Add([this] { delete renderPassColor; });
		rollback->Add([this] { delete renderPassShadowMaps; });
	}

	void GraphicsApiVulkan::CreateCommandsManager()
	{
		commandsManager = new CommandsManager(physicalDevice, logicalDevice, windowSurface, rendererConfig->maxFramesInFlight);
		rollback->Add([this]() { delete commandsManager; });
	}

	void GraphicsApiVulkan::CreateDepthBuffer() const
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
