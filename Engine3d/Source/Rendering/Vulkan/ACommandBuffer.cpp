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
}