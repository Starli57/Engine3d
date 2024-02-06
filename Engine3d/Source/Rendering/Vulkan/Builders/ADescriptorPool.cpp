#include "Pch.h"
#include "ADescriptorPool.h"

namespace AVulkan
{
	void ADescriptorPool::Create(VkDevice& logicalDevice, SwapChainData& swapChainData,
		uint32_t vpUniformBufferSize, uint32_t modelUniformBufferSize,
		VkDescriptorPool& descriptorPool) const
	{
		VkDescriptorPoolSize vpPoolSize{};
		vpPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vpPoolSize.descriptorCount = vpUniformBufferSize;

		VkDescriptorPoolSize modelPoolSize{};
		modelPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		modelPoolSize.descriptorCount = modelUniformBufferSize;

		std::vector<VkDescriptorPoolSize> poolSizes = { vpPoolSize, modelPoolSize };

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.maxSets = static_cast<uint32_t>(swapChainData.images.size());
		poolInfo.pPoolSizes = poolSizes.data();

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