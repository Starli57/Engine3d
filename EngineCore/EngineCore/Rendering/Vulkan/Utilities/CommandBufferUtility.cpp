#include "EngineCore/Pch.h"
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

		vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	}

	void BindPipeline(VkCommandBuffer& commandBuffer, Ref<PipelineVulkan> pipeline)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
	}

	void BindVertexAndIndexBuffers(VkCommandBuffer& commandBuffer, Ref<AVulkan::MeshVulkan> meshVulkan, Ref<AssetsDatabase> assetsDatabase)
	{
		VkBuffer vertexBuffers[] = { meshVulkan->GetVertexBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, meshVulkan->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
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

}