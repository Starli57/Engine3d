#include "Pch.h"
#include "CommandBufferUtility.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
	void AllocateCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount)
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

	void FreeCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers)
	{
		if (commandBuffers.size() == 0) return;
		spdlog::info("Free command buffers {0}", commandBuffers.size());

		vkFreeCommandBuffers(logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
	}

	void RecordCommandBuffer(Ref<Ecs> ecs, Ref<AssetsDatabase> assetsDatabase, Ref<AVulkan::Descriptors> descriptors, uint16_t frame,
		VkCommandBuffer& commandBuffer, std::unordered_map<std::string, Ref<PipelineVulkan>>& pipelines)
	{
		auto entities = ecs->registry->view<UboModelComponent, MeshComponent, MaterialComponent>();
		for (auto entity : entities)
		{
			auto [uboModelComponent, meshContainer, materialComponent] = entities.get<UboModelComponent, MeshComponent, MaterialComponent>(entity);
			
			if (meshContainer.GetMesh() == nullptr) continue;

			auto material = assetsDatabase->GetMaterial(materialComponent.materialIndex);
			auto pipeline = pipelines.at(material->pipelineId);

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);

			auto meshVulkan = static_pointer_cast<AVulkan::MeshVulkan> (meshContainer.GetMesh());
			auto materialVulkan = static_pointer_cast<AVulkan::MaterialVulkan>(material);

			VkBuffer vertexBuffers[] = { meshVulkan->GetVertexBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, meshVulkan->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

			auto uboModel = uboModelComponent.model;
			vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT,
				0, sizeof(UboModelComponent), &uboModel);

			auto descriptorSet = materialVulkan->descriptorSets.at(frame);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 1, &descriptorSet, 0, nullptr);

			uint32_t instanceCount = 1;
			uint32_t firstVertexIndex = 0;
			uint32_t firstInstanceIndex = 0;

			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(meshVulkan->GetIndicesCount()), 1, 0, 0, 0);
		}
	}

	void BeginCommandBuffer(VkCommandBuffer& commandBuffer) 
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;

		auto beginStatus = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		CAssert::Check(beginStatus == VK_SUCCESS, "Failed to begin recording a command buffer, status: " + beginStatus);
	}

	void EndCommandBuffer(VkCommandBuffer& commandBuffer)
	{
		auto endStatus = vkEndCommandBuffer(commandBuffer);
		CAssert::Check(endStatus == VK_SUCCESS, "Failed to end recording a command buffer, status: " + endStatus);
	}

	void BeginRenderPass(VkFramebuffer& frameBuffer, VkRenderPass& renderPass, VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent)
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

	void EndRenderPass(VkCommandBuffer& commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer);
	}
}