#include "EngineCore/Pch.h"
#include "DescriptorsAllocator.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "spdlog/spdlog.h"

namespace VulkanApi
{
	void CreateDescriptorLayout(const VkDevice& logicalDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout)
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		auto createStatus = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layout);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor set layout, status: " + createStatus);
	}

	VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(const uint32_t binding, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1)
	{
		return VkDescriptorSetLayoutBinding{
			.binding = binding,
			.descriptorType = descriptorType,
			.descriptorCount = descriptorCount,
			.stageFlags = stageFlags,
			.pImmutableSamplers = nullptr
		};
	}

	void DisposeDescriptorLayout(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout)
	{
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	}

	void CreateDescriptorPool(const VkDevice& logicalDevice, VkDescriptorPool& pool, const uint32_t capacity)
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
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor pool, status: " + createStatus);

		spdlog::info("Created descriptor pool");
	}

	void DestroyDescriptorPools(const VkDevice& logicalDevice, const VkDescriptorPool& pool)
	{
		spdlog::info("DestroyDescriptorPools");
		vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
	}

	void AllocateDescriptorSets(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout,
		const VkDescriptorPool& pool, std::vector<VkDescriptorSet>& setsReference, const uint32_t count)
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
		Engine::CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);
	}

	void WriteDescriptorSet(VkWriteDescriptorSet& writeSet, const VkDescriptorSet& descriptorSet, const uint32_t dstBinding, const uint32_t dstArrayElement,
		const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo, const VkDescriptorBufferInfo* pBufferInfo)
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