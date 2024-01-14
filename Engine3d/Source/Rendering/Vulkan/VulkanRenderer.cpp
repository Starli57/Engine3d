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

			RecordCommandBuffers();
		}
		catch (const std::exception& e)
		{
			spdlog::critical(e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void VulkanRenderer::Render()
	{
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
		ACommandBuffer().Setup(logicalDevice, commandPool, swapChainData);
	}

	//todo: replace the function somewhere
	void VulkanRenderer::RecordCommandBuffers()
	{
		spdlog::info("Record command buffers");
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		VkClearValue clearColors[] =
		{
			{0.1f, 0.1f, 0.15f, 1.0f}
		};

		for (int i = 0; i < swapChainData.commandbuffers.size(); i++)
		{
			auto commandBuffer = swapChainData.commandbuffers[i];
			auto frameBuffer = swapChainData.framebuffers[i];

			auto beginStatus = vkBeginCommandBuffer(commandBuffer, &beginInfo);
			if (beginStatus != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording a command buffer, status: " + beginStatus);
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = frameBuffer;
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainData.extent;
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = clearColors;

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			{
				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

				uint32_t vertexCount = 3;
				uint32_t instanceCount = 1;
				uint32_t firstVertexIndex = 0;
				uint32_t firstInstanceIndex = 0;
				vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertexIndex, firstInstanceIndex);
			}
			vkCmdEndRenderPass(commandBuffer);

			auto endStatus = vkEndCommandBuffer(commandBuffer);
			if (endStatus != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to end recording a command buffer, status: " + endStatus);
			}
		}
	}
}