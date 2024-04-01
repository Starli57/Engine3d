#include "Pch.h"
#include "ADescriptorSet.h"

namespace AVulkan
{
	//todo make one descriptor per material
	void ADescriptorSet::Allocate(Ref<VulkanModel> vulkanModel, 
		VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout, 
		VkImageView& textureImageView, VkSampler& textureSampler) const
	{
		auto setsCount = vulkanModel->swapChainData->uniformBuffers->size();

		std::vector<VkDescriptorSetLayout> layouts(setsCount, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(setsCount);
		allocInfo.pSetLayouts = layouts.data();

		vulkanModel->swapChainData->descriptorSets.resize(setsCount);
		auto allocateStatus = vkAllocateDescriptorSets(vulkanModel->logicalDevice, &allocInfo, vulkanModel->swapChainData->descriptorSets.data());
		CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);

		for (size_t i = 0; i < setsCount; i++)
		{
			auto descriptorSet = vulkanModel->swapChainData->descriptorSets.at(i);

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = vulkanModel->swapChainData->uniformBuffers->at(i)->buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UboViewProjectionComponent);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureImageView;
			imageInfo.sampler = textureSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSet;
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSet;
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(vulkanModel->logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
}