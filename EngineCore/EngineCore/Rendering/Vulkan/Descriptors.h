#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"

namespace AVulkan
{
	class Descriptors
	{
	public:
		VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount);

		void CreateLayout(VkDevice& logicalDevice, std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout& layout);
		void DisposeLayout(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout) const;

		VkDescriptorPool& CreateDescriptorPool(VkDevice& logicalDevice);
		void DestroyDescriptorPools(VkDevice& logicalDevice);

		void AllocateDescriptorSet(VkDevice& logicalDevice, const VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorSet& descriptorSet);

	private:
		const uint32_t maxDescriptorSets = 4096;

		uint32_t currentSetIndex = 0;
		uint32_t currentPoolIndex = 0;

		std::vector<VkDescriptorPool> descriptorPools;

		VkDescriptorPool& GetFreePool(VkDevice& logicalDevice);
	};
}
