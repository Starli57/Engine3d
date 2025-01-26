#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"

namespace AVulkan
{
	class DescriptorsManager
	{
	public:
		VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount) const;

		void CreateLayout(const VkDevice& logicalDevice, const std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout) const;
		void DisposeLayout(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout) const;

		VkDescriptorPool& CreateDescriptorPool(const VkDevice& logicalDevice);
		void DestroyDescriptorPools(const VkDevice& logicalDevice);

		void AllocateDescriptorSet(const VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorSet& descriptorSet);

	private:
		const uint32_t maxDescriptorSets = 4096;

		uint32_t currentSetIndex = 0;
		uint32_t currentPoolIndex = 0;

		std::vector<VkDescriptorPool> descriptorPools;

		VkDescriptorPool& GetFreePool(const VkDevice& logicalDevice);
	};
}
