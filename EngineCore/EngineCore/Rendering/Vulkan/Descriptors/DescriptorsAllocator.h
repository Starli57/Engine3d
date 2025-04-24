#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"

namespace AVulkan
{
	class DescriptorsAllocator
	{
	public:
		const uint32_t maxDescriptorSets = 4096;
		
		VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount) const;

		void CreateLayout(const VkDevice& logicalDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout) const;
		void DisposeLayout(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout) const;

		void CreateDescriptorPool(const VkDevice& logicalDevice, VkDescriptorPool& pool, uint32_t capacity) const;
		void DestroyDescriptorPools(const VkDevice& logicalDevice, const VkDescriptorPool& pool) const;

		void AllocateDescriptorSets(const VkDevice& logicalDevice,
			const VkDescriptorSetLayout& descriptorSetLayout, const VkDescriptorPool& pool,
			std::vector<VkDescriptorSet>& setsReference, uint32_t count) const;

		void WriteDescriptorSet(VkWriteDescriptorSet& writeSet, const VkDescriptorSet& descriptorSet, const uint32_t dstBinding, const uint32_t dstArrayElement,
			const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo, const VkDescriptorBufferInfo* pBufferInfo) const;
	};
}
