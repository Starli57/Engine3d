#include "EngineCore/Pch.h"
#include "Descriptors.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkDescriptorSetLayout& Descriptors::GetDescriptorSetLayout()
	{
		return descriptorSetLayout;
	}

	void Descriptors::CreateLayout(VkDevice& logicalDevice)
	{
		VkDescriptorSetLayoutBinding uboViewProjectionLayout{};
		uboViewProjectionLayout.binding = 0;
		uboViewProjectionLayout.descriptorCount = 1;
		uboViewProjectionLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboViewProjectionLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding uboLightLayout{};
		uboLightLayout.binding = 1;
		uboLightLayout.descriptorCount = 1;
		uboLightLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLightLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding uboCameraLayout{};
		uboCameraLayout.binding = 2;
		uboCameraLayout.descriptorCount = 1;
		uboCameraLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboCameraLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayout{};
		samplerLayout.binding = 3;
		samplerLayout.descriptorCount = 1;
		samplerLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayout.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayout.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, 4> bindings = { uboViewProjectionLayout, uboLightLayout, uboCameraLayout, samplerLayout };
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

	VkDescriptorPool& Descriptors::CreateDescriptorPool(VkDevice& logicalDevice, Ref<Rollback> rollback)
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
		spdlog::info("Created descriptor pool");

		descriptorPools.push_back(descriptorPool);

		rollback->Add([descriptorPool, logicalDevice, this]()
		{
			vkDestroyDescriptorPool(logicalDevice, descriptorPool, 0);
			spdlog::info("Disposed descriptor pool");
		});

		return descriptorPool;
	}

	VkDescriptorSet Descriptors::AllocateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback)
	{
		auto pool = GetFreePool(logicalDevice, rollback);

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

	void Descriptors::UpdateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSet& descriptorSet, 
		VkBuffer& viewProjectionDescriptorBuffer, VkDeviceSize&& viewProjectionDescriptorRange,
		VkBuffer& lightDescriptorBuffer, VkDeviceSize&& lightDescriptorRange,
		VkBuffer& cameraDescriptorBuffer, VkDeviceSize&& cameraDescriptorRange,
		VkImageView& textureImageView, VkSampler& textureSampler) const
	{
		VkDescriptorBufferInfo viewProjectionDescriptorInfo{};
		viewProjectionDescriptorInfo.buffer = viewProjectionDescriptorBuffer;
		viewProjectionDescriptorInfo.range = viewProjectionDescriptorRange;
		viewProjectionDescriptorInfo.offset = 0;

		VkDescriptorBufferInfo lightDescriptorInfo{};
		lightDescriptorInfo.buffer = lightDescriptorBuffer;
		lightDescriptorInfo.range = lightDescriptorRange;
		lightDescriptorInfo.offset = 0;

		VkDescriptorBufferInfo cameraDescriptorInfo{};
		cameraDescriptorInfo.buffer = cameraDescriptorBuffer;
		cameraDescriptorInfo.range = cameraDescriptorRange;
		cameraDescriptorInfo.offset = 0;

		std::array<VkWriteDescriptorSet, 4> descriptorWrites{};
		uint32_t descriptorsCount = 3;

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSet;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &viewProjectionDescriptorInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSet;
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &lightDescriptorInfo;

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSet;
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pBufferInfo = &cameraDescriptorInfo;

		if (textureSampler != VK_NULL_HANDLE && textureImageView != VK_NULL_HANDLE)
		{
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureImageView;
			imageInfo.sampler = textureSampler;

			descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[3].dstSet = descriptorSet;
			descriptorWrites[3].dstBinding = 3;
			descriptorWrites[3].dstArrayElement = 0;
			descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[3].descriptorCount = 1;
			descriptorWrites[3].pImageInfo = &imageInfo;
			descriptorsCount++;
		}

		vkUpdateDescriptorSets(logicalDevice, descriptorsCount, descriptorWrites.data(), 0, nullptr);
	}

	VkDescriptorPool& Descriptors::GetFreePool(VkDevice& logicalDevice, Ref<Rollback> rollback)
	{
		if ((currentSetIndex + 1) >= maxDescriptorSets)
		{
			currentPoolIndex++;
			currentSetIndex = 0;

			if (currentPoolIndex >= descriptorPools.size())
			{
				return CreateDescriptorPool(logicalDevice, rollback);
			}
		}

		if (currentPoolIndex >= descriptorPools.size()) 
		{
			return CreateDescriptorPool(logicalDevice, rollback);
		}

		return descriptorPools.at(currentPoolIndex);
	}
}