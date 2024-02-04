#include "Pch.h"
#include "ADescriptorPool.h"

namespace AVulkan
{
	void ADescriptorPool::Create(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool) const
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(swapChainData.images.size());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(swapChainData.images.size());

		auto createStatus = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
		if (createStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool, status: " + createStatus);
		}
	}

	void ADescriptorPool::Dispose(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const
	{
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}
}