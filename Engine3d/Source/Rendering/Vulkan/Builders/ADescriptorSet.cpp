#include "Pch.h"
#include "ADescriptorSet.h"

namespace AVulkan
{
	void ADescriptorSet::Allocate(Device& deviceContext, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool,
		VkDescriptorSetLayout& descriptorSetLayout) const
	{
		auto setsCount = swapChainData.images.size();

		std::vector<VkDescriptorSetLayout> layouts(setsCount, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(setsCount);
		allocInfo.pSetLayouts = layouts.data();

		swapChainData.descriptorSets.resize(setsCount);
		auto allocateStatus = vkAllocateDescriptorSets(deviceContext.GetLogicalDevice(), &allocInfo, swapChainData.descriptorSets.data());
		if (allocateStatus != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate descriptor sets, status: " + allocateStatus);
		}

		for (size_t i = 0; i < setsCount; i++)
		{
			VkDescriptorBufferInfo vpBufferInfo{};
			vpBufferInfo.buffer = deviceContext.vpUniformBuffers->at(i)->buffer;
			vpBufferInfo.offset = 0;
			vpBufferInfo.range = sizeof(UboViewProjection);

			VkWriteDescriptorSet vpWriteDesc{};
			vpWriteDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			vpWriteDesc.dstSet = swapChainData.descriptorSets[i];
			vpWriteDesc.dstBinding = 0;
			vpWriteDesc.dstArrayElement = 0;
			vpWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			vpWriteDesc.descriptorCount = 1;
			vpWriteDesc.pBufferInfo = &vpBufferInfo;
			vpWriteDesc.pImageInfo = nullptr;
			vpWriteDesc.pTexelBufferView = nullptr;

			VkDescriptorBufferInfo modelBufferInfo{};
			modelBufferInfo.buffer = deviceContext.modelUniformBuffers->at(i)->buffer;
			modelBufferInfo.offset = 0;
			modelBufferInfo.range = deviceContext.GetModelUniformAligment();

			VkWriteDescriptorSet modelWriteDesc{};
			modelWriteDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			modelWriteDesc.dstSet = swapChainData.descriptorSets[i];
			modelWriteDesc.dstBinding = 1;
			modelWriteDesc.dstArrayElement = 0;
			modelWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			modelWriteDesc.descriptorCount = 1;
			modelWriteDesc.pBufferInfo = &modelBufferInfo;
			modelWriteDesc.pImageInfo = nullptr;
			modelWriteDesc.pTexelBufferView = nullptr;

			std::vector< VkWriteDescriptorSet> sets = { vpWriteDesc , modelWriteDesc };
			vkUpdateDescriptorSets(deviceContext.GetLogicalDevice(), static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
		}
	}
}