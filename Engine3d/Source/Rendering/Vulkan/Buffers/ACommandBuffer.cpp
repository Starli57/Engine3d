#include "Pch.h"
#include "ACommandBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void ACommandBuffer::Setup(Ref<VulkanContext> vulkanContext, int buffersCount) const
	{
		spdlog::info("Create command buffer");

		auto logicalDevice = vulkanContext->GetVkLogicalDevice().get();
		auto commandBuffers = vulkanContext->GetSwapChainData()->commandBuffers;
		commandBuffers.resize(buffersCount);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *vulkanContext->GetVkCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		auto createStatus = vkAllocateCommandBuffers(*logicalDevice, &allocInfo, commandBuffers.data());
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to allocate command buffers, status: " + createStatus);
	}

	void ACommandBuffer::Record(Ref<entt::registry> ecs, Ref<VulkanContext> vulkanContext, uint32_t imageIndex) const
	{
		auto commandBuffer = vulkanContext->GetSwapChainData()->commandBuffers.at(vulkanContext->GetFrame());

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;

		std::array<VkClearValue, 2> clearColors {};
		clearColors[0].color = { 0.015f, 0.015f, 0.04f, 1.0f };
		clearColors[1].depthStencil.depth = 1.0f;

		auto beginStatus = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		CAssert::Check(beginStatus == VK_SUCCESS, "Failed to begin recording a command buffer, status: " + beginStatus);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = *vulkanContext->GetVkRenderPass();
		renderPassInfo.framebuffer = vulkanContext->GetSwapChainData()->frameBuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vulkanContext->GetSwapChainData()->extent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
		renderPassInfo.pClearValues = clearColors.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanContext->GetGraphicsPipeline()->GetPipeline());

			auto meshContainers = ecs->view<Transform, MeshContainer>();
			for (auto entity : meshContainers)
			{
				auto [transform, meshConatiner] = meshContainers.get<Transform, MeshContainer>(entity);
				auto meshVulkan = static_pointer_cast<MeshVulkan>(meshConatiner.GetMesh());

				VkBuffer vertexBuffers[] = { meshVulkan->GetVertexBuffer() };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(commandBuffer, meshVulkan->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
				
				auto pipelineLayout = vulkanContext->GetGraphicsPipeline()->GetLayout();
				auto uboModel = transform.GetUboModel();
				vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT,
					0, sizeof(UboModel), &uboModel);

				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
					0, 1, &vulkanContext->GetSwapChainData()->descriptorSets[vulkanContext->GetFrame()], 0, nullptr);

				uint32_t instanceCount = 1;
				uint32_t firstVertexIndex = 0;
				uint32_t firstInstanceIndex = 0;

				vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(meshVulkan->GetIndicesCount()), 1, 0, 0, 0);
			}
		}
		vkCmdEndRenderPass(commandBuffer);

		auto endStatus = vkEndCommandBuffer(commandBuffer);
		CAssert::Check(endStatus == VK_SUCCESS, "Failed to end recording a command buffer, status: " + endStatus);
	}
}