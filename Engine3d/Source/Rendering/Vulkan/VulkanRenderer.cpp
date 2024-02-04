#include "Pch.h"

#include <functional>

#include "VulkanRenderer.h"

namespace AVulkan
{
	VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow, Level* level, Rollback* vulkanRollback)
	{
		this->rollback = new Rollback(*vulkanRollback);
		this->window = glfwWindow;
		this->level = level;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		rollback->Dispose();
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
			CreateLevelMeshes();
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

		//todo: make it without a loop
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

	//todo: replace
	void VulkanRenderer::CreateLevelMeshes()
	{
		if (this->drawMeshes != nullptr)
		{
			spdlog::critical("DrawMeshes vector was not disposed before allocation");
		}

		this->drawMeshes = new std::vector<MeshVulkan*>();

		spdlog::info("Create render meshes");
		auto meshes = level->GetMeshes();
		auto count = meshes->size();
		for (int i = 0; i < count; i++)
		{
			AddMesh(*meshes->at(i));
		}
		rollback->Add([this]() { CleanMeshes(); });
	}

	//todo: replace
	void VulkanRenderer::CleanMeshes()
	{
		spdlog::info("Dispose render meshes");
		for (int i = 0; i < drawMeshes->size(); i++)
		{
			delete drawMeshes->at(i);
		}
		delete drawMeshes;
		drawMeshes = nullptr;
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
		ACommandBuffer().Record(frame, swapChainData.frameBuffers[imageIndex],
			renderPass, swapChainData, *graphicsPipeline, *drawMeshes);

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
		//todo: replace
		auto camera = level->GetCamera();
		camera->UpdateScreenAspectRatio(swapChainData.extent.width / (float)swapChainData.extent.height);
		camera->UpdateUboViewProjection();

		auto viewProjection = camera->GetUboViewProjection();

		//todo: replace to separated uvo
		mvp.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		memcpy(swapChainData.uniformBuffers->at(imageIndex)->bufferMapped, &viewProjection, sizeof(UboViewProjection));
	}

	void VulkanRenderer::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	void VulkanRenderer::AddMesh(Mesh& mesh)
	{
		drawMeshes->push_back(new MeshVulkan(physicalDevice, logicalDevice, swapChainData, graphicsQueue, commandPool, mesh));
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
		graphicsPipeline = new GraphicsPipeline(logicalDevice, swapChainData.extent, renderPass);
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

	void VulkanRenderer::OnFramebufferResized()
	{
		needResizeWindow = true;
	}

}