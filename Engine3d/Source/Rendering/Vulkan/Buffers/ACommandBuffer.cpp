#include "Pch.h"
#include "ACommandBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void ACommandBuffer::Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const
	{
		spdlog::info("Create command buffer");

		swapChainData.commandBuffers.resize(buffersCount);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(swapChainData.commandBuffers.size());

		auto createStatus = vkAllocateCommandBuffers(logicalDevice, &allocInfo, swapChainData.commandBuffers.data());
		if (createStatus != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to allocate command buffers, status: " + createStatus);
		}
	}

	void ACommandBuffer::Record(Device& deviceContext, uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
		SwapChainData& swapChainData, GraphicsPipeline& pipeline, std::vector<MeshVulkan*>& meshes) const
	{
		auto commandBuffer = swapChainData.commandBuffers.at(frame);

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
		renderPassInfo.renderArea.extent = swapChainData.extent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = clearColors;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());

			for (int i = 0; i < meshes.size(); i++)
			{
				auto mesh = meshes.at(i);
				VkBuffer vertexBuffers[] = { mesh->GetVertexBuffer() };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(commandBuffer, mesh->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

				uint32_t dynamicOffset = static_cast<uint32_t>(deviceContext.GetModelUniformAligment() * i);

				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), 
					0, 1, &swapChainData.descriptorSets[frame], 1, &dynamicOffset);

				uint32_t instanceCount = 1;
				uint32_t firstVertexIndex = 0;
				uint32_t firstInstanceIndex = 0;

				vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh->GetIndicesCount()), 1, 0, 0, 0);
			}
		}
		vkCmdEndRenderPass(commandBuffer);

		auto endStatus = vkEndCommandBuffer(commandBuffer);
		if (endStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to end recording a command buffer, status: " + endStatus);
		}
	}
}