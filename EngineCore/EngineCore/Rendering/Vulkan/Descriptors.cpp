#include "EngineCore/Pch.h"
#include "Descriptors.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void Descriptors::CreateLayout(VkDevice& logicalDevice, std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout)
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		auto createStatus = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layout);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create descriptor set layout, status: " + createStatus);
	}

	VkDescriptorSetLayoutBinding Descriptors::DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1)
	{
		return VkDescriptorSetLayoutBinding{
			.binding = binding,
			.descriptorType = descriptorType,
			.descriptorCount = descriptorCount,
			.stageFlags = stageFlags,
			.pImmutableSamplers = nullptr
		};
	}

	void Descriptors::DisposeLayout(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout) const
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

		descriptorPools.push_back(descriptorPool);

		spdlog::info("Created descriptor pool");

		return descriptorPool;
	}

	void Descriptors::DestroyDescriptorPools(VkDevice& logicalDevice)
	{
		spdlog::info("DestroyDescriptorPools");
		for (auto pool : descriptorPools) vkDestroyDescriptorPool(logicalDevice, pool, nullptr);
		descriptorPools.clear();
	}

	void Descriptors::AllocateDescriptorSet(VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorSet& descriptorSet)
	{
		auto pool = GetFreePool(logicalDevice);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;

		auto allocateStatus = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet);
		CAssert::Check(allocateStatus == VK_SUCCESS, "Failed to allocate descriptor sets, status: " + allocateStatus);
		
		currentSetIndex++;

		spdlog::info("Descriptor set allocated: {0}", currentSetIndex);
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