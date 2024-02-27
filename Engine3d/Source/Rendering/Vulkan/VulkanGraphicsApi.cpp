#include "Pch.h"

#include <functional>

#include "VulkanGraphicsApi.h"
#include "Rendering/Vulkan/Utilities/VkFormatUtility.h"
#include "Utilities/VkImageViewUtility.h"
#include "Utilities/VkMemoryUtility.h"

namespace AVulkan
{
	VulkanGraphicsApi::VulkanGraphicsApi(Ref<entt::registry> ecs, GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		this->ecs = ecs;
		this->window = glfwWindow;
		this->rollback = new Rollback("VulkanGraphicsApi", *vulkanRollback);
		this->swapchainRollback = CreateRef<Rollback>("SwapchainRollback");
		this->context = CreateRef<VulkanContext>();
	}

	VulkanGraphicsApi::~VulkanGraphicsApi()
	{
		delete rollback;
	}

	void VulkanGraphicsApi::Init()
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
			CreateDepthBuffer();
			CreateFrameBuffers();
			CreateCommandPool();
			CreateCommandBuffer();
			CreateUniformBuffers();
			CreateDescriptorPool();
			CreateDescriptorSets();
			CreateSyncObjects();

			rollback->Add([this]() {swapchainRollback->Dispose(); });
		}
		catch (const std::exception& e)
		{
			spdlog::critical("VulkanGraphicsApi critical error: {0}", e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void VulkanGraphicsApi::RecreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("Recreate swapchain");
		context->ResetWindowResizeFlag();
		FinanilizeRenderOperations();

		swapchainRollback->Dispose();

		CreateSwapChain();
		CreateSwapChainImageViews();
		CreateDepthBuffer();
		CreateFrameBuffers();

		auto swapChainExtent = context->GetSwapChainData()->extent;
		auto cameraEntities = ecs->view<Camera>();
		auto [mainCamera] = cameraEntities.get(cameraEntities.front());
		mainCamera.UpdateScreenAspectRatio(swapChainExtent.width / (float)swapChainExtent.height);
		mainCamera.UpdateUbo();
	}

	//todo: make refactoring of the function
	void VulkanGraphicsApi::Render()
	{
		auto frame = context->GetFrame();

		vkWaitForFences(*context->GetVkLogicalDevice().get(), 1, 
			&context->drawFences[frame], VK_TRUE, context->frameSyncTimeout);

		uint32_t imageIndex = 0;
		auto acquireStatus = vkAcquireNextImageKHR(*context->GetVkLogicalDevice().get(), 
			context->GetSwapChainData()->swapChain, context->frameSyncTimeout,
			context->imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(acquireStatus == VK_SUCCESS || acquireStatus == VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image!");

		UpdateUniformBuffer(frame);

		vkResetFences(*context->GetVkLogicalDevice().get(), 1, &context->drawFences[frame]);
		vkResetCommandBuffer(context->GetSwapChainData()->commandBuffers[frame], 0);
		ACommandBuffer().Record(ecs, context, imageIndex);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.commandBufferCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		
		submitInfo.pWaitSemaphores = &context->imageAvailableSemaphores[frame];
		submitInfo.pSignalSemaphores = &context->renderFinishedSemaphores[frame];

		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.pCommandBuffers = &context->GetSwapChainData()->commandBuffers[frame];

		auto submitStatus = vkQueueSubmit(*context->GetVkGraphicsQueue(), 1, &submitInfo, context->drawFences[frame]);
		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &context->renderFinishedSemaphores[frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &context->GetSwapChainData()->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		auto presentStatus = vkQueuePresentKHR(*context->GetVkPresentationQueue(), &presentInfo);

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR || context->GetNeedToResizeValue())
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		context->IncrementFrame();
	}

	//todo: replace
	void VulkanGraphicsApi::UpdateUniformBuffer(uint32_t imageIndex)
	{
		//todo: find most relevant camera
		auto cameraEntities = ecs->view<Camera>();
		auto [mainCamera] = cameraEntities.get(cameraEntities.front());
		auto ubo = mainCamera.GetUbo();
		auto uniformBuffer = context->GetSwapChainData()->uniformBuffers->at(imageIndex);

		memcpy(uniformBuffer->bufferMapped, &ubo, sizeof(UboViewProjection));
	}

	void VulkanGraphicsApi::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(*context->GetVkLogicalDevice());
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, swapChainData, graphicsQueue, commandPool, vertices, indices);
	}

	void VulkanGraphicsApi::CreateInstance()
	{
		AInstance().Create(instance);
		rollback->Add([this]() { AInstance().Dispose(instance); });
	}

	void VulkanGraphicsApi::CreateWindowSurface()
	{
		windowSurface = AWindowsSurface().Create(instance, *window);
		rollback->Add([this]() { AWindowsSurface().Dispose(instance, windowSurface); });
	}

	void VulkanGraphicsApi::SelectPhysicalRenderingDevice()
	{
		APhysicalDevice pdInterface;
		physicalDevice = pdInterface.GetBestRenderingDevice(instance, windowSurface);
		pdInterface.PrintDebugInformation(physicalDevice, windowSurface);
	}

	void VulkanGraphicsApi::CreateLogicalDevice()
	{
		context->SetVkLogicalDevice(ALogicalDevice().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue));
		rollback->Add([this]() { ALogicalDevice().Dispose(logicalDevice); });
	}

	void VulkanGraphicsApi::CreateSwapChain()
	{
		auto queueIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);
		ASwapChain().Create(*window, physicalDevice, logicalDevice, windowSurface, queueIndices, swapChainData);
		swapchainRollback->Add([this]() { ASwapChain().Dispose(logicalDevice, swapChainData); });
	}

	void VulkanGraphicsApi::CreateSwapChainImageViews()
	{
		AImageView().Create(logicalDevice, swapChainData);
		swapchainRollback->Add([this]() { AImageView().Dispose(logicalDevice, swapChainData); });
	}

	void VulkanGraphicsApi::CreateRenderPass()
	{
		renderPass = ARenderPass().Create(physicalDevice, logicalDevice, swapChainData.imageFormat);
		rollback->Add([this]() { ARenderPass().Dispose(logicalDevice, renderPass);; });
	}

	void VulkanGraphicsApi::CreateGraphicsPipeline()
	{
		graphicsPipeline = new GraphicsPipeline(logicalDevice, swapChainData.extent, renderPass, rollback);
		graphicsPipeline->Create(descriptorSetLayout);

		rollback->Add([this]() { delete graphicsPipeline; });
	}

	void VulkanGraphicsApi::CreateFrameBuffers()
	{
		AFrameBuffer().Create(logicalDevice, renderPass, swapChainData, depthBufferModel);
		swapchainRollback->Add([this]() { AFrameBuffer().Dispose(logicalDevice, swapChainData); });
	}

	void VulkanGraphicsApi::CreateCommandPool()
	{
		commandPool = ACommandPool().Create(logicalDevice, physicalDevice, windowSurface);
		rollback->Add([this]() { ACommandPool().Dispose(logicalDevice, commandPool); });
	}

	void VulkanGraphicsApi::CreateCommandBuffer()
	{
		ACommandBuffer().Setup(logicalDevice, commandPool, swapChainData, maxFramesDraws);
	}

	void VulkanGraphicsApi::CreateDescriptorSetLayout()
	{
		ADescriptorLayout().Create(logicalDevice, descriptorSetLayout);
		rollback->Add([this]() { ADescriptorLayout().Dispose(logicalDevice, descriptorSetLayout); });
	}

	void VulkanGraphicsApi::CreateDescriptorPool()
	{
		ADescriptorPool().Create(logicalDevice, swapChainData, descriptorPool);
		rollback->Add([this]() { ADescriptorPool().Dispose(logicalDevice, descriptorPool); });
	}

	void VulkanGraphicsApi::CreateDescriptorSets()
	{
		ADescriptorSet().Allocate(logicalDevice, swapChainData, descriptorPool, descriptorSetLayout);
	}

	void VulkanGraphicsApi::CreateDepthBuffer()
	{
		spdlog::info("Create depth buffer");

		VkFormat depthFormat = VkFormatUtility::FindDepthBufferFormat(physicalDevice);
		depthBufferModel = CreateRef<DepthBufferModel>();
		
		depthBufferModel->image = AImage().Create(
			physicalDevice, logicalDevice, 
			swapChainData.extent.width, swapChainData.extent.height, depthFormat, 
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			depthBufferModel->imageMemory);

		VkImageViewUtility::Create(logicalDevice, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			depthBufferModel->image, depthBufferModel->imageView);

		swapchainRollback->Add([this]()
		{
			VkImageViewUtility::Destroy(logicalDevice, depthBufferModel->imageView);
			AImage().Destroy(logicalDevice, depthBufferModel->image);
			VkMemoryUtility::FreeDeviceMemory(logicalDevice, depthBufferModel->imageMemory);
			depthBufferModel.reset();
		});
	}

	void VulkanGraphicsApi::CreateUniformBuffers()
	{
		auto buffersCount = swapChainData.images.size();
		swapChainData.uniformBuffers = new std::vector<UniformBufferVulkan*>();
		swapChainData.uniformBuffers->reserve(buffersCount);
		for (int i = 0; i < buffersCount; i++)
		{
			swapChainData.uniformBuffers->push_back(new UniformBufferVulkan(physicalDevice, logicalDevice));
		}

		rollback->Add([this]() { DisposeUniformBuffers(); });
	}

	void VulkanGraphicsApi::DisposeUniformBuffers()
	{
		for (int i = 0; i < swapChainData.uniformBuffers->size(); i++)
		{
			delete swapChainData.uniformBuffers->at(i);
		}
		delete swapChainData.uniformBuffers;
	}

	//todo: replace 
	void VulkanGraphicsApi::CreateSyncObjects()
	{
		context->imageAvailableSemaphores.resize(context->maxFramesDraws);
		context->renderFinishedSemaphores.resize(context->maxFramesDraws);
		context->drawFences.resize(context->maxFramesDraws);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < context->maxFramesDraws; i++)
		{
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &context->imageAvailableSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, context->imageAvailableSemaphores[i], nullptr); });

			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &context->renderFinishedSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, context->renderFinishedSemaphores[i], nullptr); });

			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &context->drawFences[i]);
			rollback->Add([i, this]() { vkDestroyFence(logicalDevice, context->drawFences[i], nullptr); });
		}
	}

	void VulkanGraphicsApi::OnFramebufferResized()
	{
		needResizeWindow = true;
	}

}