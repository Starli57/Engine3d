#include "EngineCore/Pch.h"
#include "CommandBufferUtility.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
	void AllocateCommandBuffers(const VkDevice& logicalDevice, const VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount)
	{
		commandBuffers.resize(buffersCount);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		auto createStatus = vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data());
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to allocate command buffers, status: " + createStatus);
	}

	void FreeCommandBuffers(const VkDevice& logicalDevice, const VkCommandPool& commandPool, const std::vector<VkCommandBuffer>& commandBuffers)
	{
		if (commandBuffers.size() == 0) return;
		vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	}

	void BindPipeline(const VkCommandBuffer& commandBuffer, const Ref<PipelineVulkan>& pipeline)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
	}

	void BindVertexAndIndexBuffers(const VkCommandBuffer& commandBuffer, const int32_t meshVulkan, const Ref<AssetsDatabaseVulkan>& assetsDatabase)
	{
		VkBuffer vertexBuffers[] = { assetsDatabase->vertexBuffers.at(meshVulkan)};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, assetsDatabase->indexBuffers.at(meshVulkan), 0, VK_INDEX_TYPE_UINT32);
	}

	void BeginCommandBuffer(const VkCommandBuffer& commandBuffer) 
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;

		auto beginStatus = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		CAssert::Check(beginStatus == VK_SUCCESS, "Failed to begin recording a command buffer, status: " + beginStatus);
	}

	void EndCommandBuffer(const VkCommandBuffer& commandBuffer)
	{
		auto endStatus = vkEndCommandBuffer(commandBuffer);
		CAssert::Check(endStatus == VK_SUCCESS, "Failed to end recording a command buffer, status: " + endStatus);
	}

}