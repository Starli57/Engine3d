#include "Pch.h"
#include "ACommandBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void ACommandBuffer::Allocate(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount) const
	{
		spdlog::info("Create command buffer");

		commandBuffers.resize(buffersCount);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		auto createStatus = vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data());
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to allocate command buffers, status: " + createStatus);
	}

	void ACommandBuffer::Begin(VkCommandBuffer& commandBuffer) const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;

		auto beginStatus = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		CAssert::Check(beginStatus == VK_SUCCESS, "Failed to begin recording a command buffer, status: " + beginStatus);
	}

	void ACommandBuffer::End(VkCommandBuffer& commandBuffer) const
	{
		auto endStatus = vkEndCommandBuffer(commandBuffer);
		CAssert::Check(endStatus == VK_SUCCESS, "Failed to end recording a command buffer, status: " + endStatus);
	}

	void ACommandBuffer::BeginRenderPass(VkFramebuffer& frameBuffer, VkRenderPass& renderPass, VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent) const
	{
		std::array<VkClearValue, 2> clearColors{};
		clearColors[0].color = { 0.015f, 0.015f, 0.04f, 1.0f };
		clearColors[1].depthStencil.depth = 1.0f;

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = frameBuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vkExtent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
		renderPassInfo.pClearValues = clearColors.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void ACommandBuffer::EndRenderPass(VkCommandBuffer& commandBuffer) const
	{
		vkCmdEndRenderPass(commandBuffer);
	}


	void ACommandBuffer::Record(Ref<entt::registry> ecs, Ref<Descriptors> descriptors, uint16_t frame,
		VkCommandBuffer& commandBuffer, GraphicsPipeline& pipeline) const
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipeline());

		auto entities = ecs->view<UboModelComponent, MeshComponent, MaterialComponent>();
		for (auto entity : entities)
		{
			auto [uboModelComponent, meshConatiner, materialComponent] = entities.get<UboModelComponent, MeshComponent, MaterialComponent>(entity);
			auto meshVulkan = static_pointer_cast<MeshVulkan>(meshConatiner.GetMesh());
			auto textureVulkan = static_pointer_cast<TextureVulkan>(materialComponent.GetMaterial()->mainTexture);

			VkBuffer vertexBuffers[] = { meshVulkan->GetVertexBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, meshVulkan->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

			auto uboModel = uboModelComponent.model;
			vkCmdPushConstants(commandBuffer, pipeline.GetLayout(), VK_SHADER_STAGE_VERTEX_BIT,
				0, sizeof(UboModelComponent), &uboModel);

			auto descriptorSet = textureVulkan->descriptorSets.at(frame);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetLayout(), 0, 1, &descriptorSet, 0, nullptr);

			uint32_t instanceCount = 1;
			uint32_t firstVertexIndex = 0;
			uint32_t firstInstanceIndex = 0;

			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(meshVulkan->GetIndicesCount()), 1, 0, 0, 0);
		}
	}
}