#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class Descriptors
	{
	public:
		VkDescriptorSetLayout& GetDescriptorSetLayout();

		void CreateLayout(VkDevice& logicalDevice);
		void DisposeLayout(VkDevice& logicalDevice) const;

		VkDescriptorPool& CreateDescriptorPool(VkDevice& logicalDevice);
		void ResetDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
		void DisposeDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
		void DisposeAllDescriptorPools(VkDevice& logicalDevice);

		VkDescriptorSet AllocateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout);
		void UpdateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSet& descriptorSet, 
			VkBuffer& descriptorBuffer, VkImageView& textureImageView, VkSampler& textureSampler) const;

	private:
		const uint32_t maxDescriptorSets = 4096;

		uint32_t currentSetIndex = 0;
		uint32_t currentPoolIndex = 0;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorPool> descriptorPools;

		VkDescriptorPool& GetFreePool(VkDevice& logicalDevice);
	};
}