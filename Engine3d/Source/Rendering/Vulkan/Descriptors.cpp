#include "Pch.h"
#include "Descriptors.h"
#include "SharedLib/CustomAssert.h"

namespace AVulkan
{
	VkDescriptorSetLayout& Descriptors::GetDescriptorSetLayout()
	{
		return descriptorSetLayout;
	}

	void Descriptors::CreateLayout(VkDevice& logicalDevice) const
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		auto createStatus = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor set layout, status: " + createStatus);
	}

	void Descriptors::DisposeLayout(VkDevice& logicalDevice) const
	{
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	}

	void Descriptors::CreateDescriptorPool(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool) const
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = maxDescriptorSets;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = maxDescriptorSets;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = maxDescriptorSets;

		auto createStatus = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor pool, status: " + createStatus);
	}

	void Descriptors::ResetDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const
	{
		vkResetDescriptorPool(logicalDevice, descriptorPool, 0);
	}

	void Descriptors::DisposeDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const
	{
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}

	void Descriptors::AllocateDescriptorSet(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorSetLayout& descriptorSetLayout,
		VkImageView& textureImageView, VkSampler& textureSampler)
	{
		auto pool = GetFreePool(logicalDevice, swapChainData);

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;

		auto allocateStatus = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet);
		CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);
		
		descriptorSets.at(currentSetIndex) = descriptorSet;
		currentSetIndex++;
	}

	void Descriptors::UpdateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSet& descriptorSet, VkBuffer& descriptorBuffer,
		VkImageView& textureImageView, VkSampler& textureSampler) const
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = descriptorBuffer;
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

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	VkDescriptorPool& Descriptors::GetFreePool(VkDevice& logicalDevice, SwapChainData& swapChainData)
	{
		if (currentSetIndex >= maxDescriptorSets)
		{
			currentPoolIndex++;
			currentSetIndex = 0;

			if (currentPoolIndex >= descriptorPools.size())
			{
				VkDescriptorPool pool = VK_NULL_HANDLE;
				CreateDescriptorPool(logicalDevice, swapChainData, pool);
				descriptorPools.push_back(pool);
				return pool;
			}
		}

		return descriptorPools.at(currentPoolIndex);
	}
}