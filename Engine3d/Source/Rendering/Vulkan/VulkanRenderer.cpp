#include "Pch.h"
#include "VulkanRenderer.h"
#include <functional>

namespace AVulkan
{
	VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		rollback = vulkanRollback;
		window = glfwWindow;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		rollback->Dispose();
	}

	void VulkanRenderer::Initialize()
	{
		try
		{
			CreateInstance();
			CreateWindowSurface();
			SelectPhysicalRenderingDevice();
			CreateLogicalDevice();
			CreateSwapChain();
			CreateSwapChainImageViews();
			CreateRenderPass();
			CreateGraphicsPipeline();
			CreateFrameBuffers();
			CreateCommandPool();
			CreateCommandBuffer();
			CreateSyncObjects();
		}
		catch (const std::exception& e)
		{
			spdlog::critical(e.what());
			rollback->Dispose();
			throw e;
		}
	}

	//todo: make refactoring of the function
	void VulkanRenderer::Render()
	{
		auto timeout = UINT64_MAX;//todo: setup real timeout

		vkWaitForFences(logicalDevice, 1, &drawFences[frame], VK_TRUE, timeout);
		vkResetFences(logicalDevice, 1, &drawFences[frame]);

		uint32_t imageIndex = 0;
		vkAcquireNextImageKHR(logicalDevice, swapChainData.swapChain, timeout, imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(swapChainData.commandbuffers[frame], 0);
		ACommandBuffer().Record(swapChainData.commandbuffers[frame], swapChainData.framebuffers[imageIndex],
			renderPass, swapChainData.extent, graphicsPipeline);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.commandBufferCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		
		submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frame];
		submitInfo.pSignalSemaphores = &renderFinishedSemaphores[frame];

		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.pCommandBuffers = &swapChainData.commandbuffers[frame];

		auto submitStatus = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[frame]);
		if (submitStatus != VK_SUCCESS)  throw std::runtime_error("Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChainData.swapChain;
		presentInfo.pImageIndices = &imageIndex;
		auto presentStatus = vkQueuePresentKHR(presentationQueue, &presentInfo);
		if (presentStatus != VK_SUCCESS)  throw std::runtime_error("Failed to present draw command buffer, status: " + presentStatus);

		frame = (frame + 1) % maxFramesDraws;
	}

	void VulkanRenderer::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	void VulkanRenderer::CreateInstance()
	{
		AInstance().Create(instance);
		rollback->Add([this]() { AInstance().Dispose(instance); });
	}

	void VulkanRenderer::CreateWindowSurface()
	{
		windowSurface = AWindowsSurface().Create(instance, *window);
		rollback->Add([this]() { AWindowsSurface().Dispose(instance, windowSurface); });
	}

	void VulkanRenderer::SelectPhysicalRenderingDevice()
	{
		APhysicalDevice pdInterface;
		physicalDevice = pdInterface.GetBestRenderingDevice(instance, windowSurface);
		pdInterface.PrintDebugInformation(physicalDevice, windowSurface);
	}

	void VulkanRenderer::CreateLogicalDevice()
	{
		logicalDevice = ALogicalDevice().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
		rollback->Add([this]() { ALogicalDevice().Dispose(logicalDevice); });
	}

	void VulkanRenderer::CreateSwapChain()
	{
		auto queueIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);
		swapChainData = ASwapChain().Create(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);
		rollback->Add([this]() { ASwapChain().Dispose(logicalDevice, swapChainData); });
	}

	void VulkanRenderer::CreateSwapChainImageViews()
	{
		AImageView().Create(logicalDevice, swapChainData);
		rollback->Add([this]() { AImageView().Dispose(logicalDevice, swapChainData); });
	}

	void VulkanRenderer::CreateRenderPass()
	{
		renderPass = ARenderPass().Create(logicalDevice, swapChainData.imageFormat);
		rollback->Add([this]() { ARenderPass().Dispose(logicalDevice, renderPass);; });
	}

	void VulkanRenderer::CreateGraphicsPipeline()
	{
		graphicsPipeline = AGraphicsPipeline(logicalDevice, swapChainData.extent, renderPass).Create();
		rollback->Add([this]() { AGraphicsPipeline(logicalDevice, swapChainData.extent, renderPass).Dispose(graphicsPipeline); });
	}

	void VulkanRenderer::CreateFrameBuffers()
	{
		AFrameBuffer().Create(logicalDevice, renderPass, swapChainData);
		rollback->Add([this]() { AFrameBuffer().Dispose(logicalDevice, swapChainData); });
	}

	void VulkanRenderer::CreateCommandPool()
	{
		commandPool = ACommandPool().Create(logicalDevice, physicalDevice, windowSurface);
		rollback->Add([this]() { ACommandPool().Dispose(logicalDevice, commandPool); });
	}

	void VulkanRenderer::CreateCommandBuffer()
	{
		ACommandBuffer().Setup(logicalDevice, commandPool, swapChainData, maxFramesDraws);
	}

	//todo: replace 
	void VulkanRenderer::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(maxFramesDraws);
		renderFinishedSemaphores.resize(maxFramesDraws);
		drawFences.resize(maxFramesDraws);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < maxFramesDraws; i++) 
		{
			auto imageSemaphoreStatus = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr); });
			if (imageSemaphoreStatus != VK_SUCCESS) throw std::runtime_error("Failed to create image sync semaphor");

			auto renderFinishedSemaphoreStatus = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr); });
			if (renderFinishedSemaphoreStatus != VK_SUCCESS) throw std::runtime_error("Failed to create render finished sync semaphor");;

			auto inFlightFenceStatus = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &drawFences[i]);
			rollback->Add([i, this]() { vkDestroyFence(logicalDevice, drawFences[i], nullptr); });
			if (inFlightFenceStatus != VK_SUCCESS) throw std::runtime_error("Failed to create sync fence!");
		}
	}

}