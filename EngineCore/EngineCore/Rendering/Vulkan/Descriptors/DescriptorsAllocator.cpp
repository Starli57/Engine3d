#include "EngineCore/Pch.h"
#include "DescriptorsAllocator.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "spdlog/spdlog.h"

using namespace EngineCore;

namespace AVulkan
{
	void DescriptorsAllocator::CreateLayout(const VkDevice& logicalDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout) const
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		auto createStatus = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layout);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor set layout, status: " + createStatus);
	}

	VkDescriptorSetLayoutBinding DescriptorsAllocator::DescriptorSetLayoutBinding(const uint32_t binding, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1) const
	{
		return VkDescriptorSetLayoutBinding{
			.binding = binding,
			.descriptorType = descriptorType,
			.descriptorCount = descriptorCount,
			.stageFlags = stageFlags,
			.pImmutableSamplers = nullptr
		};
	}

	void DescriptorsAllocator::DisposeLayout(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout) const
	{
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	}

	void DescriptorsAllocator::CreateDescriptorPool(const VkDevice& logicalDevice, VkDescriptorPool& pool, const uint32_t capacity) const
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = capacity;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = capacity;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = capacity;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		const auto createStatus = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &pool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor pool, status: " + createStatus);

		spdlog::info("Created descriptor pool");
	}

	void DescriptorsAllocator::DestroyDescriptorPools(const VkDevice& logicalDevice, const VkDescriptorPool& pool) const
	{
		spdlog::info("DestroyDescriptorPools");
		vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
	}

	void DescriptorsAllocator::AllocateDescriptorSets(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout,
		const VkDescriptorPool& pool, std::vector<VkDescriptorSet>& setsReference, const uint32_t count) const
	{
		spdlog::info("Allocate Descriptor Sets {}", count);
		std::vector<VkDescriptorSetLayout> layouts(count, descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = count;
		allocInfo.pSetLayouts = layouts.data();

		setsReference.resize(count);
		const auto allocateStatus = vkAllocateDescriptorSets(logicalDevice, &allocInfo, setsReference.data());
		CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);
	}

	void DescriptorsAllocator::WriteDescriptorSet(VkWriteDescriptorSet& writeSet, const VkDescriptorSet& descriptorSet, const uint32_t dstBinding, const uint32_t dstArrayElement,
		const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo, const VkDescriptorBufferInfo* pBufferInfo) const
	{
		writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeSet.dstSet = descriptorSet;
		writeSet.dstBinding = dstBinding;
		writeSet.dstArrayElement = dstArrayElement;
		writeSet.descriptorType = descriptorType;
		writeSet.descriptorCount = descriptorCount;
		writeSet.pImageInfo = pImageInfo;
		writeSet.pBufferInfo = pBufferInfo;
	}
}