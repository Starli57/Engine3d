#include "Pch.h"
#include "Descriptors.h"
#include "EngineShared/CustomAssert.h"
#include "EngineShared/Components/UboViewProjectionComponent.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkDescriptorSetLayout& Descriptors::GetDescriptorSetLayout()
	{
		return descriptorSetLayout;
	}

	void Descriptors::CreateLayout(VkDevice& logicalDevice)
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

	VkDescriptorPool& Descriptors::CreateDescriptorPool(VkDevice& logicalDevice)
	{
		VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

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
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		auto createStatus = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor pool, status: " + createStatus);
		spdlog::info("Descriptor pool allocated");

		descriptorPools.push_back(descriptorPool);
		return descriptorPool;
	}

	void Descriptors::ResetDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const
	{
		vkResetDescriptorPool(logicalDevice, descriptorPool, 0);
	}

	void Descriptors::DisposeDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const
	{
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
		spdlog::info("Descriptor pool disposed");
	}

	void Descriptors::DisposeAllDescriptorPools(VkDevice& logicalDevice)
	{
		for (auto pool : descriptorPools) DisposeDescriptorPool(logicalDevice, pool);
		descriptorPools.clear();
	}

	VkDescriptorSet Descriptors::AllocateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout)
	{
		auto pool = GetFreePool(logicalDevice);

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;

		auto allocateStatus = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet);
		CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);
		
		currentSetIndex++;

		spdlog::info("Descriptor set allocated: {0}", currentSetIndex);
		return descriptorSet;
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

	VkDescriptorPool& Descriptors::GetFreePool(VkDevice& logicalDevice)
	{
		if ((currentSetIndex + 1) >= maxDescriptorSets)
		{
			currentPoolIndex++;
			currentSetIndex = 0;

			if (currentPoolIndex >= descriptorPools.size())
			{
				return CreateDescriptorPool(logicalDevice);
			}
		}

		if (currentPoolIndex >= descriptorPools.size()) 
		{
			return CreateDescriptorPool(logicalDevice);
		}

		return descriptorPools.at(currentPoolIndex);
	}
}