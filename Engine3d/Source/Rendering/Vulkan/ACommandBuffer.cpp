#include "Pch.h"
#include "ACommandBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void ACommandBuffer::Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData) const
	{
		spdlog::info("Create command buffer");

		swapChainData.commandbuffers.resize(swapChainData.imageViews.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(swapChainData.commandbuffers.size());

		auto createStatus = vkAllocateCommandBuffers(logicalDevice, &allocInfo, swapChainData.commandbuffers.data());
		if (createStatus != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to allocate command buffers, status: " + createStatus);
		}
	}

	void ACommandBuffer::Record(VkCommandBuffer& commandBuffer, VkFramebuffer& frameBuffer, VkRenderPass& renderPass, 
		VkExtent2D& swapchainExtent, VkPipeline& pipeline) const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;

		VkClearValue clearColors[] =
		{
			{0.015f, 0.015f, 0.04f, 1.0f}
		};

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
		renderPassInfo.renderArea.extent = swapchainExtent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = clearColors;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

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