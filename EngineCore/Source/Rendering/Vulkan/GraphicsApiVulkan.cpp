#include "Pch.h"

#include <functional>

#include "GraphicsApiVulkan.h"

namespace AVulkan
{
	GraphicsApiVulkan::GraphicsApiVulkan(Ref<Ecs> ecs, Ref<AssetsDatabase> assetDatabase, Ref<ProjectSettigns> projectSettings, 
		GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		this->ecs = ecs;
		this->assetDatabase = assetDatabase;
		this->projectSettings = projectSettings;
		this->window = glfwWindow;
		this->descriptors = CreateRef<Descriptors>();
		this->pipelinesCollection = CreateRef<PipelinesCollection>(projectSettings);
		this->rollback = CreateRef<Rollback>("GraphicsApiVulkan", *vulkanRollback);
	}

	GraphicsApiVulkan::~GraphicsApiVulkan()
	{
		DisposePipelines();
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
			CreateSwapChain();
			CreateSwapChainImageViews();
			CreateRenderPass();
			CreateDescriptorSetLayout();
			CreateGraphicsPipelines();
			CreateDepthBuffer();
			CreateFrameBuffers();
			CreateCommandPool();
			CreateCommandBuffer();
			CreateTextureSampler();
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

		swapChain->Recreate();

		for (auto pipelineConfig : pipelinesCollection->pipelinesConfigs)
		{
			auto& pipeline = pipelines.at(pipelineConfig.first);
			pipeline = GraphicsPipelineUtility().ReCreate(pipeline, pipelineConfig.second, logicalDevice, renderPass,
				swapChainData->extent, descriptors->GetDescriptorSetLayout());
		}
	}

	//todo: make refactoring of the function
	void GraphicsApiVulkan::Render()
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
		VkUtils::RecordCommandBuffer(ecs, assetDatabase, descriptors, frame, commandBuffer, pipelines);
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

	void GraphicsApiVulkan::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(logicalDevice);
	}

	Ref<Mesh> GraphicsApiVulkan::CreateMesh(const std::filesystem::path& meshPath)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, graphicsQueue, commandPool, meshPath, rollback);
	}

	Ref<Mesh> GraphicsApiVulkan::CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
	{
		return CreateRef<MeshVulkan>(physicalDevice, logicalDevice, graphicsQueue, commandPool, vertices, indices, rollback);
	}

	Ref<Texture> GraphicsApiVulkan::CreateTexture(std::filesystem::path& textureFilePath)
	{
		return CreateRef<TextureVulkan>(physicalDevice, logicalDevice, graphicsQueue, commandPool, textureFilePath, rollback);
	}

	Ref<Material> GraphicsApiVulkan::CreateMaterial(std::string& pipelineId)
	{
		return CreateRef<MaterialVulkan>(pipelineId, assetDatabase, physicalDevice, logicalDevice, descriptors,
			textureSampler, descriptors->GetDescriptorSetLayout(), rollback);
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
		swapChain = CreateRef<SwapChain>(rollback, *window, physicalDevice, logicalDevice, windowSurface, graphicsQueue, swapChainData);

		swapChain->CreateSwapchain();

		rollback->Add([this] {swapChain->Dispose(); });
	}

	void GraphicsApiVulkan::CreateSwapChainImageViews()
	{
		swapChain->CreateSwapChainImageViews();
	}

	void GraphicsApiVulkan::CreateRenderPass()
	{
		renderPass = VkUtils::CreateRenderPass(physicalDevice, logicalDevice, swapChainData->imageFormat);
		rollback->Add([this]() { VkUtils::DisposeRenderPass(logicalDevice, renderPass);; });
	}

	void GraphicsApiVulkan::CreateGraphicsPipelines()
	{
		pipelines.reserve(pipelinesCollection->pipelinesConfigs.size());

		for (auto config : pipelinesCollection->pipelinesConfigs)
		{
			GraphicsPipelineUtility pipelineUtility;
			auto pipeline = pipelineUtility.Create(config.second, logicalDevice, renderPass, 
				swapChainData->extent, descriptors->GetDescriptorSetLayout());

			pipelines.emplace(config.first, pipeline);
		}
	}

	void GraphicsApiVulkan::CreateFrameBuffers()
	{
		swapChain->CreateFrameBuffers(renderPass);
	}

	void GraphicsApiVulkan::CreateCommandPool()
	{
		commandPool = VkUtils::CreateCommandPool(logicalDevice, physicalDevice, windowSurface);
		rollback->Add([this]() { VkUtils::DisposeCommandPool(logicalDevice, commandPool); });
	}

	void GraphicsApiVulkan::CreateCommandBuffer()
	{
		VkUtils::AllocateCommandBuffers(logicalDevice, commandPool, commandBuffers, maxFramesInFlight);
		rollback->Add([this]() { VkUtils::FreeCommandBuffers(logicalDevice, commandPool, commandBuffers); });
	}

	void GraphicsApiVulkan::CreateDescriptorSetLayout()
	{
		descriptors->CreateLayout(logicalDevice);
		rollback->Add([this]() { descriptors->DisposeLayout(logicalDevice); });
	}

	void GraphicsApiVulkan::CreateDepthBuffer()
	{
		spdlog::info("Create depth buffer");
		swapChain->CreateDepthBuffer(commandPool);
	}

	void GraphicsApiVulkan::CreateTextureSampler()
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

	void GraphicsApiVulkan::UpdateUniformBuffer(uint32_t frame)
	{
		//todo: find most relevant camera
		auto viewProjectionEntries = ecs->registry->view<UboViewProjectionComponent>();
		auto [viewProjectionComponent] = viewProjectionEntries.get(viewProjectionEntries.front());

		auto materialEntries = ecs->registry->view<MaterialComponent>();
		auto lightEntries = ecs->registry->view<UboDiffuseLightComponent>();
		for (auto materialEntry : materialEntries)
		{
			auto materialComponent = materialEntries.get<MaterialComponent>(materialEntry);
			auto materialVulkan = static_pointer_cast<MaterialVulkan>(assetDatabase->GetMaterial(materialComponent.materialIndex));

			memcpy(materialVulkan->uboViewProjection.at(frame)->bufferMapped, &viewProjectionComponent, sizeof(UboViewProjectionComponent));

			for (auto entity : lightEntries)
			{
				auto& positionComponent = lightEntries.get<UboDiffuseLightComponent>(entity);
				memcpy(materialVulkan->uboLights.at(frame)->bufferMapped, &positionComponent, sizeof(UboDiffuseLightComponent));
			}

			materialVulkan->UpdateDescriptors(frame);
		}
	}

	void GraphicsApiVulkan::DisposePipelines()
	{
		GraphicsPipelineUtility pipelineUtility;
		for (auto& pipeline : pipelines)
		{
			pipelineUtility.Dispose(pipeline.second, logicalDevice);
		}
		pipelines.clear();

		spdlog::info("All graphics pipelines disposed");
	}

	void GraphicsApiVulkan::CreateSyncObjects()
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
			VkUtils::CreateVkSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i], rollback);
			VkUtils::CreateVkSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i], rollback);
			VkUtils::CreateVkFence(logicalDevice, &fenceInfo, nullptr, &drawFences[i], rollback);
		}
	}
}