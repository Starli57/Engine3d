#include "Pch.h"

#include <functional>

#include "VulkanGraphicsApi.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "Rendering/Vulkan/Entities/TextureVulkan.h"
#include "Builders/AImageView.h"
#include "Utilities/MemoryUtility.h"
#include "Utilities/FormatUtility.h"

namespace AVulkan
{
	VulkanGraphicsApi::VulkanGraphicsApi(Ref<entt::registry> ecs, Ref<ProjectSettigns> projectSettings, GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		this->ecs = ecs;
		this->projectSettings = projectSettings;
		this->window = glfwWindow;
		this->rollback = CreateRef<Rollback>("VulkanGraphicsApi", *vulkanRollback);
		this->descriptors = CreateRef<Descriptors>();
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
			CreateSyncObjects();
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
		FinanilizeRenderOperations();

		swapChain->Recreate();
	}

	//todo: make refactoring of the function
	void VulkanGraphicsApi::Render()
	{

		auto commandBuffer = commandBuffers[frame];

		vkWaitForFences(logicalDevice, 1, &drawFences[frame], VK_TRUE, frameSyncTimeout);

		auto acquireStatus = vkAcquireNextImageKHR(logicalDevice, swapChainData->swapChain, frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);
		
		if (acquireStatus == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
			return;
		}
		
		CAssert::Check(acquireStatus == VK_SUCCESS || acquireStatus == VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image!");

		UpdateUniformBuffer(frame);

		vkResetFences(logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(commandBuffer, 0);

		VkUtils::BeginCommandBuffer(commandBuffer);
		VkUtils::BeginRenderPass(swapChainData->frameBuffers[imageIndex], renderPass, commandBuffer, swapChainData->extent);
		VkUtils::RecordCommandBuffer(ecs, descriptors, frame, commandBuffer, *graphicsPipeline);
		VkUtils::EndRenderPass(commandBuffer);
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
		
		frame = (frame + 1) % maxFramesInFlight;
	}

	void VulkanGraphicsApi::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(const std::string& meshPath)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, graphicsQueue, commandPool, meshPath);
	}

	Ref<Mesh> VulkanGraphicsApi::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, graphicsQueue, commandPool, vertices, indices);
	}

	Ref<Texture> VulkanGraphicsApi::CreateTexture(TextureId textureId)
	{
		return CreateRef<TextureVulkan>(projectSettings, physicalDevice, logicalDevice, descriptors,
			descriptors->GetDescriptorSetLayout(), textureSampler, graphicsQueue, commandPool, textureId, rollback);
	}

	void VulkanGraphicsApi::CreateInstance()
	{
		VkUtils::CreateInstance(instance);
		rollback->Add([this]() { VkUtils::DisposeInstance(instance); });
	}

	void VulkanGraphicsApi::CreateWindowSurface()
	{
		windowSurface = VkUtils::CreateSurface(instance, *window);
		rollback->Add([this]() { VkUtils::DisposeSurface(instance, windowSurface); });
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
		rollback->Add([this]() 
		{
			descriptors->DisposeAllDescriptorPools(logicalDevice);
			ALogicalDevice().Dispose(logicalDevice); 
		});
	}

	void VulkanGraphicsApi::CreateSwapChain()
	{
		auto queueIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);

		swapChainData = CreateRef<SwapChainData>();
		swapChain = CreateRef<SwapChain>(rollback, *window, physicalDevice, logicalDevice, windowSurface,
			queueIndices, graphicsQueue, swapChainData);

		swapChain->CreateSwapchain();

		rollback->Add([this] {swapChain->Dispose(); });
	}

	void VulkanGraphicsApi::CreateSwapChainImageViews()
	{
		swapChain->CreateSwapChainImageViews();
	}

	void VulkanGraphicsApi::CreateRenderPass()
	{
		renderPass = VkUtils::CreateRenderPass(physicalDevice, logicalDevice, swapChainData->imageFormat);
		rollback->Add([this]() { VkUtils::DisposeRenderPass(logicalDevice, renderPass);; });
	}

	void VulkanGraphicsApi::CreateGraphicsPipeline()
	{
		graphicsPipeline = new GraphicsPipeline(projectSettings, logicalDevice, swapChainData->extent, renderPass, rollback);
		graphicsPipeline->Create(descriptors->GetDescriptorSetLayout());

		rollback->Add([this]() { delete graphicsPipeline; });
	}

	void VulkanGraphicsApi::CreateFrameBuffers()
	{
		swapChain->CreateFrameBuffers(renderPass);
	}

	void VulkanGraphicsApi::CreateCommandPool()
	{
		commandPool = VkUtils::CreateCommandPool(logicalDevice, physicalDevice, windowSurface);
		rollback->Add([this]() { VkUtils::DisposeCommandPool(logicalDevice, commandPool); });
	}

	void VulkanGraphicsApi::CreateCommandBuffer()
	{
		VkUtils::AllocateCommandBuffer(logicalDevice, commandPool, commandBuffers, maxFramesInFlight);
	}

	void VulkanGraphicsApi::CreateDescriptorSetLayout()
	{
		descriptors->CreateLayout(logicalDevice);
		rollback->Add([this]() { descriptors->DisposeLayout(logicalDevice); });
	}

	void VulkanGraphicsApi::CreateDepthBuffer()
	{
		spdlog::info("Create depth buffer");
		swapChain->CreateDepthBuffer(commandPool);
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

	void VulkanGraphicsApi::UpdateUniformBuffer(uint32_t frame)
	{
		//todo: find most relevant camera
		auto uboEntries = ecs->view<UboViewProjectionComponent>();
		auto [uboComponent] = uboEntries.get(uboEntries.front());

		auto materialEntries = ecs->view<MaterialComponent>();
		for (auto materialEntry : materialEntries)
		{
			auto materialComponent = materialEntries.get<MaterialComponent>(materialEntry);
			auto textureVulkan = static_pointer_cast<TextureVulkan>(materialComponent.GetMaterial()->mainTexture);

			memcpy(textureVulkan->uniformBuffers.at(frame)->bufferMapped, &uboComponent, sizeof(UboViewProjectionComponent));
		}
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
}