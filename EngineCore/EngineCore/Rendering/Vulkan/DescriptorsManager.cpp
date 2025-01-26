#include "EngineCore/Pch.h"
#include "DescriptorsManager.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void DescriptorsManager::CreateLayout(const VkDevice& logicalDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout) const
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		auto createStatus = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layout);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor set layout, status: " + createStatus);
	}

	VkDescriptorSetLayoutBinding DescriptorsManager::DescriptorSetLayoutBinding(const uint32_t binding, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1) const
	{
		return VkDescriptorSetLayoutBinding{
			.binding = binding,
			.descriptorType = descriptorType,
			.descriptorCount = descriptorCount,
			.stageFlags = stageFlags,
			.pImmutableSamplers = nullptr
		};
	}

	void DescriptorsManager::DisposeLayout(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout) const
	{
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	}

	VkDescriptorPool& DescriptorsManager::CreateDescriptorPool(const VkDevice& logicalDevice)
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

		const auto createStatus = vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor pool, status: " + createStatus);

		descriptorPools.push_back(descriptorPool);

		spdlog::info("Created descriptor pool");

		return descriptorPools.at(descriptorPools.size() - 1);
	}

	void DescriptorsManager::DestroyDescriptorPools(const VkDevice& logicalDevice)
	{
		spdlog::info("DestroyDescriptorPools");
		for (auto pool : descriptorPools) vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
		descriptorPools.clear();
	}

	void DescriptorsManager::AllocateDescriptorSet(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorSet& descriptorSet)
	{
		auto pool = GetFreePool(logicalDevice);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;

		const auto allocateStatus = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet);
		CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);
		
		currentSetIndex++;

		spdlog::info("Descriptor set allocated: {0}", currentSetIndex);
	}

	VkDescriptorPool& DescriptorsManager::GetFreePool(const VkDevice& logicalDevice)
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