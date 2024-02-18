#include "Pch.h"

#include <functional>

#include "VulkanRenderer.h"

namespace AVulkan
{
	VulkanRenderer::VulkanRenderer(Ref<entt::registry> ecs, GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		this->ecs = ecs;
		this->rollback = new Rollback("VulkanRenderer", *vulkanRollback);
		this->window = glfwWindow;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		delete rollback;
	}

	void VulkanRenderer::Init()
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
			CreateDescriptorSetLayout();
			CreateGraphicsPipeline();
			CreateFrameBuffers();
			CreateCommandPool();
			CreateCommandBuffer();
			CreateUniformBuffers();
			CreateDescriptorPool();
			CreateDescriptorSets();
			CreateSyncObjects();
		}
		catch (const std::exception& e)
		{
			spdlog::critical("VulkanRenderer critical error: {0}", e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void VulkanRenderer::RecreateSwapChain()
	{
		/// to recreate spawnchain it's enough to dispose and recreate only the next 3 components:
		/// swapchain, spwanchainImageView and commandBuffers
		/// but for better architecture we dispose all components and start the initialization from beginning

		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);

		//todo: make it with no loop
		while (width == 0 || height == 0) 
		{
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("Recreate swapchain");
		needResizeWindow = false;
		vkDeviceWaitIdle(logicalDevice);
		rollback->Dispose();
		Init();
	}

	//todo: make refactoring of the function
	void VulkanRenderer::Render()
	{
		vkWaitForFences(logicalDevice, 1, &drawFences[frame], VK_TRUE, frameSyncTimeout);

		uint32_t imageIndex = 0;
		auto acquireStatus = vkAcquireNextImageKHR(logicalDevice, swapChainData.swapChain, frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);

		UpdateUniformBuffer(frame);

		vkResetFences(logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(swapChainData.commandBuffers[frame], 0);
		ACommandBuffer().Record(ecs, frame, swapChainData.frameBuffers[imageIndex],
			renderPass, swapChainData, *graphicsPipeline);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.commandBufferCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		
		submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frame];
		submitInfo.pSignalSemaphores = &renderFinishedSemaphores[frame];

		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.pCommandBuffers = &swapChainData.commandBuffers[frame];

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

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR || needResizeWindow)
		{
			RecreateSwapChain();
		}
		else if (presentStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present draw command buffer, status: " + presentStatus);
		}

		frame = (frame + 1) % maxFramesDraws;
	}

	//todo: replace
	void VulkanRenderer::UpdateUniformBuffer(uint32_t imageIndex)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		//todo: find most relevant camera
		auto cameraEntities = ecs->view<Camera>();
		auto [mainCamera] = cameraEntities.get(cameraEntities.front());
		mainCamera.UpdateScreenAspectRatio(swapChainData.extent.width / (float)swapChainData.extent.height);
		mainCamera.UpdateUboViewProjection();
		auto mvp = mainCamera.GetUboViewProjection();

		memcpy(swapChainData.uniformBuffers->at(imageIndex)->bufferMapped, &mvp, sizeof(UboViewProjection));
	}

	void VulkanRenderer::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	Ref<Mesh> VulkanRenderer::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, swapChainData, graphicsQueue, commandPool, vertices, indices);
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
		graphicsPipeline = new GraphicsPipeline(logicalDevice, swapChainData.extent, renderPass, rollback);
		graphicsPipeline->Create(descriptorSetLayout);

		rollback->Add([this]() { delete graphicsPipeline; });
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

	void VulkanRenderer::CreateDescriptorSetLayout()
	{
		ADescriptorLayout().Create(logicalDevice, descriptorSetLayout);
		rollback->Add([this]() { ADescriptorLayout().Dispose(logicalDevice, descriptorSetLayout); });
	}

	void VulkanRenderer::CreateDescriptorPool()
	{
		ADescriptorPool().Create(logicalDevice, swapChainData, descriptorPool);
		rollback->Add([this]() { ADescriptorPool().Dispose(logicalDevice, descriptorPool); });
	}

	void VulkanRenderer::CreateDescriptorSets()
	{
		ADescriptorSet().Allocate(logicalDevice, swapChainData, descriptorPool, descriptorSetLayout);
	}

	void VulkanRenderer::CreateUniformBuffers()
	{
		auto buffersCount = swapChainData.images.size();
		swapChainData.uniformBuffers = new std::vector<UniformBufferVulkan*>();
		swapChainData.uniformBuffers->reserve(buffersCount);
		for (int i = 0; i < buffersCount; i++)
		{
			swapChainData.uniformBuffers->push_back(new UniformBufferVulkan(physicalDevice, logicalDevice));
		}

		rollback->Add([this]() {DisposeUniformBuffers(); });
	}

	void VulkanRenderer::DisposeUniformBuffers()
	{
		for (int i = 0; i < swapChainData.uniformBuffers->size(); i++)
		{
			delete swapChainData.uniformBuffers->at(i);
		}
		delete swapChainData.uniformBuffers;
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
			if (imageSemaphoreStatus != VK_SUCCESS) throw std::runtime_error("Failed to create image sync semaphor");
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr); });

			auto renderFinishedSemaphoreStatus = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
			if (renderFinishedSemaphoreStatus != VK_SUCCESS) throw std::runtime_error("Failed to create render finished sync semaphor");;
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr); });

			auto inFlightFenceStatus = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &drawFences[i]);
			if (inFlightFenceStatus != VK_SUCCESS) throw std::runtime_error("Failed to create sync fence!");
			rollback->Add([i, this]() { vkDestroyFence(logicalDevice, drawFences[i], nullptr); });
		}
	}

	void VulkanRenderer::OnFramebufferResized()
	{
		needResizeWindow = true;
	}

}