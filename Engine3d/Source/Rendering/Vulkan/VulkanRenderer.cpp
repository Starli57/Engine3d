#include "Pch.h"

#include <functional>

#include "VulkanRenderer.h"

namespace AVulkan
{
	VulkanRenderer::VulkanRenderer(entt::registry* ecs, GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		this->ecs = ecs;
		this->rollback = new Rollback(*vulkanRollback);
		this->window = glfwWindow;
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
			CreateDevice();
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

		auto logicalDevice = device->GetLogicalDevice();

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
		auto meshContainers = ecs->view<Transform, MeshContainer>();
		for (auto entity : meshContainers)
		{
			auto [transform, meshConatiner] = meshContainers.get<Transform, MeshContainer>(entity);
			auto mesh = meshConatiner.GetMesh();
			AddMesh(mesh);
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
		auto logicalDevice = device->GetLogicalDevice();
		auto graphicsQueue = device->GetGraphicsQueue();
		auto presentationQueue = device->GetPresentationQueue();

		vkWaitForFences(logicalDevice, 1, &drawFences[frame], VK_TRUE, frameSyncTimeout);

		uint32_t imageIndex = 0;
		auto acquireStatus = vkAcquireNextImageKHR(logicalDevice, swapChainData.swapChain, frameSyncTimeout,
			imageAvailableSemaphores[frame], VK_NULL_HANDLE, &imageIndex);

		UpdateUniformBuffers(frame);

		vkResetFences(logicalDevice, 1, &drawFences[frame]);
		vkResetCommandBuffer(swapChainData.commandBuffers[frame], 0);
		ACommandBuffer().Record(*device, frame, swapChainData.frameBuffers[imageIndex],
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
	void VulkanRenderer::UpdateUniformBuffers(uint32_t imageIndex)
	{
		//todo: find most relevant camera
		auto cameraEntities = ecs->view<Camera>();
		auto [mainCamera] = cameraEntities.get(cameraEntities.front());
		mainCamera.UpdateScreenAspectRatio(swapChainData.extent.width / (float)swapChainData.extent.height);
		mainCamera.UpdateUboViewProjection();

		auto viewProjection = mainCamera.GetUboViewProjection();
		memcpy(device->vpUniformBuffers->at(imageIndex)->bufferMapped, &viewProjection, sizeof(UboViewProjection));

		auto meshContainers = ecs->view<Transform, MeshContainer>();
		int i = 0;
		for (auto entity : meshContainers)
		{
			auto [transform, meshConatiner] = meshContainers.get<Transform, MeshContainer>(entity);
			auto uboModel = transform.GetUboModel();
			////
			UboModel* modelPlacement = (UboModel*)((uint64_t)device->GetModelUniformTransfer() + (i * device->GetModelUniformAligment()));
			*modelPlacement = uboModel;

			i++;
		}

		memcpy(device->modelUniformBuffers->at(imageIndex)->bufferMapped, device->GetModelUniformTransfer(),
			device->GetModelUniformAligment() * drawMeshes->size());
	}

	void VulkanRenderer::FinanilizeRenderOperations()
	{
		vkDeviceWaitIdle(device->GetLogicalDevice());
	}

	void VulkanRenderer::AddMesh(Ref<Mesh> mesh)
	{
		auto physicalDevice = device->GetPhysicalDevice();
		auto logicalDevice = device->GetLogicalDevice();
		auto graphicsQueue = device->GetGraphicsQueue();
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

	void VulkanRenderer::CreateDevice()
	{
		device = new Device();
		device->Create(instance, windowSurface, *rollback);

		rollback->Add([this]()
		{
			device->Dispose();
			delete device;
		});
	}

	void VulkanRenderer::CreateSwapChain()
	{
		auto physicalDevice = device->GetPhysicalDevice();
		auto logicalDevice = device->GetLogicalDevice();
		auto queueIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);
		swapChainData = ASwapChain().Create(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			ASwapChain().Dispose(logicalDevice, swapChainData); 
		});
	}

	void VulkanRenderer::CreateSwapChainImageViews()
	{
		auto logicalDevice = device->GetLogicalDevice();
		AImageView().Create(logicalDevice, swapChainData);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			AImageView().Dispose(logicalDevice, swapChainData); 
		});
	}

	void VulkanRenderer::CreateRenderPass()
	{
		auto logicalDevice = device->GetLogicalDevice();
		renderPass = ARenderPass().Create(logicalDevice, swapChainData.imageFormat);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			ARenderPass().Dispose(logicalDevice, renderPass);
		});
	}

	void VulkanRenderer::CreateGraphicsPipeline()
	{
		auto logicalDevice = device->GetLogicalDevice();
		graphicsPipeline = new GraphicsPipeline(logicalDevice, swapChainData.extent, renderPass);
		graphicsPipeline->Create(descriptorSetLayout);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			delete graphicsPipeline; 
		});
	}

	void VulkanRenderer::CreateFrameBuffers()
	{
		auto logicalDevice = device->GetLogicalDevice();
		AFrameBuffer().Create(logicalDevice, renderPass, swapChainData);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			AFrameBuffer().Dispose(logicalDevice, swapChainData); 
		});
	}

	void VulkanRenderer::CreateCommandPool()
	{
		auto physicalDevice = device->GetPhysicalDevice();
		auto logicalDevice = device->GetLogicalDevice();
		commandPool = ACommandPool().Create(logicalDevice, physicalDevice, windowSurface);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			ACommandPool().Dispose(logicalDevice, commandPool); 
		});
	}

	void VulkanRenderer::CreateCommandBuffer()
	{
		auto logicalDevice = device->GetLogicalDevice();
		ACommandBuffer().Setup(logicalDevice, commandPool, swapChainData, maxFramesDraws);
	}

	void VulkanRenderer::CreateDescriptorSetLayout()
	{
		auto logicalDevice = device->GetLogicalDevice();
		ADescriptorLayout().Create(logicalDevice, descriptorSetLayout);

		rollback->Add([this]() 
		{
			auto logicalDevice = device->GetLogicalDevice();
			ADescriptorLayout().Dispose(logicalDevice, descriptorSetLayout); 
		});
	}

	void VulkanRenderer::CreateDescriptorPool()
	{
		auto logicalDevice = device->GetLogicalDevice();
		ADescriptorPool().Create(logicalDevice, swapChainData,
			static_cast<uint32_t>(device->vpUniformBuffers->size()),
			static_cast<uint32_t>(device->modelUniformBuffers->size()),
			descriptorPool);

		rollback->Add([this]()
		{
			auto logicalDevice = device->GetLogicalDevice();
			ADescriptorPool().Dispose(logicalDevice, descriptorPool);
		});
	}

	void VulkanRenderer::CreateDescriptorSets()
	{
		ADescriptorSet().Allocate(*device, swapChainData, descriptorPool, descriptorSetLayout);
	}

	void VulkanRenderer::CreateUniformBuffers()
	{
		auto physicalDevice = device->GetPhysicalDevice();
		auto logicalDevice = device->GetLogicalDevice();
		auto buffersCount = swapChainData.images.size();
		auto vpBufferSize = sizeof(UboViewProjection);
		device->vpUniformBuffers = new std::vector<UniformBufferVulkan*>();
		device->vpUniformBuffers->reserve(buffersCount);

		device->modelUniformBuffers = new std::vector<UniformBufferVulkan*>();
		device->modelUniformBuffers->reserve(buffersCount);
		auto modelBufferSize = device->GetModelUniformAligment() * device->modelsPerDynamicUniform;

		for (int i = 0; i < buffersCount; i++)
		{
			device->vpUniformBuffers->push_back(new UniformBufferVulkan(physicalDevice, logicalDevice, vpBufferSize));
			device->modelUniformBuffers->push_back(new UniformBufferVulkan(physicalDevice, logicalDevice, modelBufferSize));
		}

		rollback->Add([this]() { DisposeUniformBuffers(); });
	}

	void VulkanRenderer::DisposeUniformBuffers()
	{
		for (int i = 0; i < device->vpUniformBuffers->size(); i++)
		{
			delete device->vpUniformBuffers->at(i);
		}
		delete device->vpUniformBuffers;

		for (int i = 0; i < device->modelUniformBuffers->size(); i++)
		{
			delete device->modelUniformBuffers->at(i);
		}
		delete device->modelUniformBuffers;
	}

	//todo: replace 
	void VulkanRenderer::CreateSyncObjects()
	{
		auto logicalDevice = device->GetLogicalDevice();

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
			rollback->Add([i, this]()
			{
					auto logicalDevice = device->GetLogicalDevice();
				vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
			});
			if (imageSemaphoreStatus != VK_SUCCESS) throw std::runtime_error("Failed to create image sync semaphor");

			auto renderFinishedSemaphoreStatus = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
			rollback->Add([i, this]()
			{
				auto logicalDevice = device->GetLogicalDevice();
				vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
			});
			if (renderFinishedSemaphoreStatus != VK_SUCCESS) throw std::runtime_error("Failed to create render finished sync semaphor");;

			auto inFlightFenceStatus = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &drawFences[i]);
			rollback->Add([i, this]()
			{
				auto logicalDevice = device->GetLogicalDevice();
				vkDestroyFence(logicalDevice, drawFences[i], nullptr);
			});
		}
	}

	void VulkanRenderer::OnFramebufferResized()
	{
		needResizeWindow = true;
	}

}