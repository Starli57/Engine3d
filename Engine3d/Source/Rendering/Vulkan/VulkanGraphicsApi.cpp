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

		this->model = CreateRef<VulkanModel>();
		model->window = glfwWindow;
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
		glfwGetFramebufferSize(model->window, &width, &height);

		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(model->window, &width, &height);
			glfwWaitEvents();
		}

		spdlog::info("Recreate swapchain");
		model->needResizeWindow = false;
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
		vkWaitForFences(model->logicalDevice, 1, &model->drawFences[model->frame], VK_TRUE, model->frameSyncTimeout);

		uint32_t imageIndex = 0;
		auto acquireStatus = vkAcquireNextImageKHR(model->logicalDevice, model->swapChainData->swapChain, model->frameSyncTimeout,
			model->imageAvailableSemaphores[model->frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(acquireStatus == VK_SUCCESS || acquireStatus == VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image!");

		UpdateUniformBuffer(model->frame);

		vkResetFences(model->logicalDevice, 1, &model->drawFences[model->frame]);
		vkResetCommandBuffer(model->swapChainData->commandBuffers[model->frame], 0);
		ACommandBuffer().Record(model, ecs, model->swapChainData->frameBuffers[imageIndex]);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.commandBufferCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		
		submitInfo.pWaitSemaphores = &model->imageAvailableSemaphores[model->frame];
		submitInfo.pSignalSemaphores = &model->renderFinishedSemaphores[model->frame];

		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.pCommandBuffers = &model->swapChainData->commandBuffers[model->frame];

		auto submitStatus = vkQueueSubmit(model->graphicsQueue, 1, &submitInfo, model->drawFences[model->frame]);
		CAssert::Check(submitStatus == VK_SUCCESS, "Failed to submit draw command buffer, status: " + submitStatus);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &model->renderFinishedSemaphores[model->frame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &model->swapChainData->swapChain;
		presentInfo.pImageIndices = &imageIndex;

		auto presentStatus = vkQueuePresentKHR(model->presentationQueue, &presentInfo);

		if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR || model->needResizeWindow)
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(presentStatus == VK_SUCCESS, "Failed to present draw command buffer, status: " + presentStatus);
		
		model->frame = (model->frame + 1) % model->maxFramesInFlight;
	}

	//todo: replace
	void VulkanGraphicsApi::UpdateUniformBuffer(uint32_t imageIndex)
	{
		//todo: find most relevant camera
		auto entries = ecs->view<UboViewProjectionComponent>();
		auto [uboComponent] = entries.get(entries.front());

		memcpy(model->swapChainData->uniformBuffers->at(imageIndex)->bufferMapped, &uboComponent, sizeof(UboViewProjectionComponent));
	}

	void VulkanGraphicsApi::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(model->logicalDevice);
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(const std::string& meshPath)
	{
		return CreateRef<MeshVulkan>(model, meshPath);
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
	{
		return CreateRef<MeshVulkan>(model, vertices, indices);
	}

	Ref<Texture> VulkanGraphicsApi::CreateTexture(TextureId textureId)
	{
		return CreateRef<TextureVulkan>(
			projectSettings, model, 
			model->descriptorPool, model->descriptorSetLayout,
			model->textureSampler, model->commandPool, textureId);
	}

	void VulkanGraphicsApi::CreateInstance()
	{
		AInstance().Create(model->instance);
		rollback->Add([this]() { AInstance().Dispose(model->instance); });
	}

	void VulkanGraphicsApi::CreateWindowSurface()
	{
		model->windowSurface = AWindowsSurface().Create(model);
		rollback->Add([this]() { AWindowsSurface().Dispose(model); });
	}

	void VulkanGraphicsApi::SelectPhysicalRenderingDevice()
	{
		APhysicalDevice pdInterface;
		model->physicalDevice = pdInterface.GetBestRenderingDevice(model->instance, model->windowSurface);
		pdInterface.PrintDebugInformation(model->physicalDevice, model->windowSurface);
	}

	void VulkanGraphicsApi::CreateLogicalDevice()
	{
		ALogicalDevice().Create(model);
		rollback->Add([this]() { ALogicalDevice().Dispose(model->logicalDevice); });
	}

	void VulkanGraphicsApi::CreateSwapChain()
	{
		auto queueIndices = APhysicalDevice().GetQueueFamilies(model->physicalDevice, model->windowSurface);
		ASwapChain().Create(model, queueIndices);
		swapchainRollback->Add([this]() { ASwapChain().Dispose(model->logicalDevice, model->swapChainData); });
	}

	void VulkanGraphicsApi::CreateSwapChainImageViews()
	{
		AImageView().Create(model->logicalDevice, model->swapChainData);
		swapchainRollback->Add([this]() { AImageView().Dispose(model->logicalDevice, model->swapChainData); });
	}

	void VulkanGraphicsApi::CreateRenderPass()
	{
		ARenderPass().Create(model);
		rollback->Add([this]() { ARenderPass().Dispose(model->logicalDevice, model->renderPass);; });
	}

	void VulkanGraphicsApi::CreateGraphicsPipeline()
	{
		model->graphicsPipeline = CreateRef<GraphicsPipeline>(projectSettings, model->logicalDevice, 
			model->swapChainData->extent, model->renderPass, rollback);
		model->graphicsPipeline->Create(model->descriptorSetLayout);

		rollback->Add([this]() { model->graphicsPipeline.reset(); });
	}

	void VulkanGraphicsApi::CreateFrameBuffers()
	{
		AFrameBuffer().Create(model);
		swapchainRollback->Add([this]() { AFrameBuffer().Dispose(model); });
	}

	void VulkanGraphicsApi::CreateCommandPool()
	{
		model->commandPool = ACommandPool().Create(model);
		rollback->Add([this]() { ACommandPool().Dispose(model->logicalDevice, model->commandPool); });
	}

	void VulkanGraphicsApi::CreateCommandBuffer()
	{
		ACommandBuffer().Setup(model, model->maxFramesInFlight);
	}

	void VulkanGraphicsApi::CreateDescriptorSetLayout()
	{
		ADescriptorLayout().Create(model->logicalDevice, model->descriptorSetLayout);
		rollback->Add([this]() { ADescriptorLayout().Dispose(model->logicalDevice, model->descriptorSetLayout); });
	}

	void VulkanGraphicsApi::CreateDescriptorPool()
	{
		ADescriptorPool().Create(model->logicalDevice, model->swapChainData, model->descriptorPool);
		rollback->Add([this]() { ADescriptorPool().Dispose(model->logicalDevice, model->descriptorPool); });
	}

	void VulkanGraphicsApi::CreateDescriptorSets()
	{
	}

	void VulkanGraphicsApi::CreateDepthBuffer()
	{
		spdlog::info("Create depth buffer");

		VkFormat depthFormat = VkFormatUtility::FindDepthBufferFormat(model->physicalDevice);
		model->swapChainData->depthBufferModel = CreateRef<DepthBufferModel>();
		
		model->swapChainData->depthBufferModel->image = AImage(model, model->commandPool).Create(
			model->swapChainData->extent.width, model->swapChainData->extent.height, depthFormat,
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			model->swapChainData->depthBufferModel->imageMemory);

		VkImageViewUtility::Create(model->logicalDevice, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
			model->swapChainData->depthBufferModel->image, model->swapChainData->depthBufferModel->imageView);

		swapchainRollback->Add([this]()
		{
			VkImageViewUtility::Destroy(model->logicalDevice, model->swapChainData->depthBufferModel->imageView);
			AImage(model, model->commandPool).Destroy(model->swapChainData->depthBufferModel->image);
			VkMemoryUtility::FreeDeviceMemory(model->logicalDevice, model->swapChainData->depthBufferModel->imageMemory);
			model->swapChainData->depthBufferModel.reset();
		});
	}

	void VulkanGraphicsApi::CreateTextureSampler()
	{
		//todo: replace logic to helper class
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(model->physicalDevice, &properties);

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

		auto createStatus = vkCreateSampler(model->logicalDevice, &samplerInfo, nullptr, &model->textureSampler);
		CAssert::Check(createStatus == VK_SUCCESS, "Textures sampler can't be created");

		rollback->Add([this]() { vkDestroySampler(model->logicalDevice, model->textureSampler, nullptr); });
	}

	void VulkanGraphicsApi::CreateUniformBuffers()
	{
		auto buffersCount = model->swapChainData->images.size();
		model->swapChainData->uniformBuffers = new std::vector<UniformBufferVulkan*>();
		model->swapChainData->uniformBuffers->reserve(buffersCount);
		for (int i = 0; i < buffersCount; i++)
		{
			model->swapChainData->uniformBuffers->push_back(new UniformBufferVulkan(model->physicalDevice, model->logicalDevice));
		}

		rollback->Add([this]() { DisposeUniformBuffers(); });
	}

	void VulkanGraphicsApi::DisposeUniformBuffers()
	{
		for (int i = 0; i < model->swapChainData->uniformBuffers->size(); i++)
		{
			delete model->swapChainData->uniformBuffers->at(i);
		}
		delete model->swapChainData->uniformBuffers;
	}

	//todo: replace 
	void VulkanGraphicsApi::CreateSyncObjects()
	{
		model->imageAvailableSemaphores.resize(model->maxFramesInFlight);
		model->renderFinishedSemaphores.resize(model->maxFramesInFlight);
		model->drawFences.resize(model->maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < model->maxFramesInFlight; i++) 
		{
			vkCreateSemaphore(model->logicalDevice, &semaphoreInfo, nullptr, &model->imageAvailableSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(model->logicalDevice, model->imageAvailableSemaphores[i], nullptr); });

			vkCreateSemaphore(model->logicalDevice, &semaphoreInfo, nullptr, &model->renderFinishedSemaphores[i]);
			rollback->Add([i, this]() { vkDestroySemaphore(model->logicalDevice, model->renderFinishedSemaphores[i], nullptr); });

			vkCreateFence(model->logicalDevice, &fenceInfo, nullptr, &model->drawFences[i]);
			rollback->Add([i, this]() { vkDestroyFence(model->logicalDevice, model->drawFences[i], nullptr); });
		}
	}

	void VulkanGraphicsApi::OnFramebufferResized()
	{
		model->needResizeWindow = true;
	}

}