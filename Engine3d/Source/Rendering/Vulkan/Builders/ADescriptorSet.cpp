#include "Pch.h"
#include "ADescriptorSet.h"

namespace AVulkan
{
	void ADescriptorSet::Allocate(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
		VkDescriptorSetLayout& descriptorSetLayout) const
	{
		auto setsCount = swapChainData.uniformBuffers->size();

		std::vector<VkDescriptorSetLayout> layouts(setsCount, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(setsCount);
		allocInfo.pSetLayouts = layouts.data();

		swapChainData.descriptorSets.resize(setsCount);
		auto allocateStatus = vkAllocateDescriptorSets(logicalDevice, &allocInfo, swapChainData.descriptorSets.data());
		if (allocateStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate descriptor sets, status: " + allocateStatus);
		}

		for (size_t i = 0; i < setsCount; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = swapChainData.uniformBuffers->at(i)->buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UboViewProjection);

			VkWriteDescriptorSet mvpWriteDesc{};
			mvpWriteDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			mvpWriteDesc.dstSet = swapChainData.descriptorSets[i];
			mvpWriteDesc.dstBinding = 0;
			mvpWriteDesc.dstArrayElement = 0;
			mvpWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			mvpWriteDesc.descriptorCount = 1;
			mvpWriteDesc.pBufferInfo = &bufferInfo;
			mvpWriteDesc.pImageInfo = nullptr;
			mvpWriteDesc.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(logicalDevice, 1, &mvpWriteDesc, 0, nullptr);
		}
	}
}