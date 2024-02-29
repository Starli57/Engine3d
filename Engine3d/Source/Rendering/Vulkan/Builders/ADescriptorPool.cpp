#include "Pch.h"
#include "ADescriptorPool.h"

namespace AVulkan
{
	void ADescriptorPool::Create(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool) const
	{
		//todo: is descriptors count must be multiplied by number of textures ? 

		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainData.images.size());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainData.images.size());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapChainData.images.size());

		auto createStatus = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor pool, status: " + createStatus);
	}

	void ADescriptorPool::Dispose(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const
	{
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}
}