#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Models/SwapchainContext.h"

namespace VulkanApi
{
	const uint32_t maxDescriptorSets = 4096;
	
	VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount);

	void CreateDescriptorLayout(const VkDevice& logicalDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout);
	void DisposeDescriptorLayout(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout);

	void CreateDescriptorPool(const VkDevice& logicalDevice, VkDescriptorPool& pool, uint32_t capacity);
	void DestroyDescriptorPools(const VkDevice& logicalDevice, const VkDescriptorPool& pool);

	void AllocateDescriptorSets(const VkDevice& logicalDevice,
		const VkDescriptorSetLayout& descriptorSetLayout, const VkDescriptorPool& pool,
		std::vector<VkDescriptorSet>& setsReference, uint32_t count);

	void WriteDescriptorSet(VkWriteDescriptorSet& writeSet, const VkDescriptorSet& descriptorSet, const uint32_t dstBinding, const uint32_t dstArrayElement,
		const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo, const VkDescriptorBufferInfo* pBufferInfo);
}
