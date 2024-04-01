#include "Pch.h"

#include <functional>

#include "VulkanGraphicsApi.h"
#include "Rendering/Vulkan/Utilities/VkFormatUtility.h"
#include "Utilities/VkImageViewUtility.h"
#include "Utilities/VkMemoryUtility.h"

namespace AVulkan
{
	VulkanGraphicsApi::VulkanGraphicsApi(Ref<entt::registry> ecs, Ref<ProjectSettigns> projectSettings, GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		this->ecs = ecs;
		this->projectSettings = projectSettings;
		this->rollback = CreateRef<Rollback>("VulkanGraphicsApi", *vulkanRollback);
		this->swapchainRollback = CreateRef<Rollback>("SwapchainRollback");
		this->window = glfwWindow;
	}

	VulkanGraphicsApi::~VulkanGraphicsApi()
	{
		rollback.reset();
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
			CreateTextureSampler();
			CreateUniformBuffers();
			CreateDescriptorPool();
			CreateDescriptorSets();
			CreateSyncObjects();

			rollback->Add([this]() { swapchainRollback->Dispose(); });
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
		needResizeWindow = false;
		FinanilizeRenderOperations();

		swapchainRollback->Dispose();

		CreateSwapChain();
		CreateSwapChainImageViews();
		CreateDepthBuffer();
		CreateFrameBuffers();
	}

	//todo: make refactoring of the function
	void VulkanGraphicsApi::Render()
	{
		vkWaitForFences(logicalDevice, 1, &drawFences[frame], VK_TRUE, frameSyncTimeout);

		uint32_t imageIndex = 0;
		auto acquireStatus = vkAcquireNextImageKHR(logicalDevice, swapChainData.swapChain, frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(acquireStatus == VK_SUCCESS || acquireStatus == VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image!");

		UpdateUniformBuffer(frame);

		vkResetFences(logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(commandBuffers[frame], 0);
		ACommandBuffer().Record(ecs, frame, swapChainData.frameBuffers[imageIndex],
			renderPass, commandBuffers, descriptorSets, *graphicsPipeline, swapChainData.extent);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.commandBufferCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		
		submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frame];
		submitInfo.pSignalSemaphores = &renderFinishedSemaphores[frame];

		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.pCommandBuffers = &commandBuffers[frame];

		auto submitStatus = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[frame]);
		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

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
			return;
		}
		
		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		frame = (frame + 1) % maxFramesInFlight;
	}

	//todo: replace
	void VulkanGraphicsApi::UpdateUniformBuffer(uint32_t imageIndex)
	{
		//todo: find most relevant camera
		auto entries = ecs->view<UboViewProjectionComponent>();
		auto [uboComponent] = entries.get(entries.front());

		memcpy(uniformBuffers.at(imageIndex)->bufferMapped, &uboComponent, sizeof(UboViewProjectionComponent));
	}

	void VulkanGraphicsApi::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(const std::string& meshPath)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, swapChainData, graphicsQueue, commandPool, meshPath);
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, swapChainData, graphicsQueue, commandPool, vertices, indices);
	}

	Ref<Texture> VulkanGraphicsApi::CreateTexture(TextureId textureId)
	{
		return CreateRef<TextureVulkan>(projectSettings, physicalDevice, logicalDevice, uniformBuffers,
			descriptorSets, descriptorPool, descriptorSetLayout, textureSampler, graphicsQueue, commandPool, textureId);
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
		logicalDevice = ALogicalDevice().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
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
		graphicsPipeline = new GraphicsPipeline(projectSettings, logicalDevice, swapChainData.extent, renderPass, rollback);
		graphicsPipeline->Create(descriptorSetLayout);

		rollback->Add([this]() { delete graphicsPipeline; });
	}

	void VulkanGraphicsApi::CreateFrameBuffers()
	{
		AFrameBuffer().Create(logicalDevice, renderPass, swapChainData, depthBufferModel);
		swapchainRollback->Add([this]() { AFrameBuffer().Dispose(logicalDevice, swapChainData.frameBuffers); });
	}

	void VulkanGraphicsApi::CreateCommandPool()
	{
		commandPool = ACommandPool().Create(logicalDevice, physicalDevice, windowSurface);
		rollback->Add([this]() { ACommandPool().Dispose(logicalDevice, commandPool); });
	}

	void VulkanGraphicsApi::CreateCommandBuffer()
	{
		ACommandBuffer().Setup(logicalDevice, commandPool, commandBuffers, maxFramesInFlight);
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
	}

	void VulkanGraphicsApi::CreateDepthBuffer()
	{
		spdlog::info("Create depth buffer");

		VkFormat depthFormat = VkFormatUtility::FindDepthBufferFormat(physicalDevice);
		depthBufferModel = CreateRef<DepthBufferModel>();
		
		depthBufferModel->image = AImage(physicalDevice, logicalDevice, graphicsQueue, commandPool).Create(
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
			AImage(physicalDevice, logicalDevice, graphicsQueue, commandPool).Destroy(depthBufferModel->image);
			VkMemoryUtility::FreeDeviceMemory(logicalDevice, depthBufferModel->imageMemory);
			depthBufferModel.reset();
		});
	}

	void VulkanGraphicsApi::CreateTextureSampler()
	{
		//todo: replace logic to helper class
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		auto createStatus = vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &textureSampler);
		CAssert::Check(createStatus == VK_SUCCESS, "Textures sampler can't be created");

		rollback->Add([this]() { vkDestroySampler(logicalDevice, textureSampler, nullptr); });
	}

	void VulkanGraphicsApi::CreateUniformBuffers()
	{
		auto buffersCount = swapChainData.images.size();
		uniformBuffers.reserve(buffersCount);
		for (int i = 0; i < buffersCount; i++)
		{
			uniformBuffers.push_back(new UniformBufferVulkan(physicalDevice, logicalDevice));
		}

		rollback->Add([this]() { DisposeUniformBuffers(); });
	}

	void VulkanGraphicsApi::DisposeUniformBuffers()
	{
		for (int i = 0; i < uniformBuffers.size(); i++)
		{
			delete uniformBuffers.at(i);
		}
		uniformBuffers.resize(0);
	}

	//todo: replace 
	void VulkanGraphicsApi::CreateSyncObjects()
	{
		imageAvailableSemaphores.resize(maxFramesInFlight);
		renderFinishedSemaphores.resize(maxFramesInFlight);
		drawFences.resize(maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < maxFramesInFlight; i++) 
		{
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr); });

			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr); });

			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &drawFences[i]);
			rollback->Add([i, this]() { vkDestroyFence(logicalDevice, drawFences[i], nullptr); });
		}
	}

	void VulkanGraphicsApi::OnFramebufferResized()
	{
		needResizeWindow = true;
	}

}