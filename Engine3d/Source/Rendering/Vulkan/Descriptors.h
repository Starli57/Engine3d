#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	class Descriptors
	{
	public:
		VkDescriptorSetLayout& GetDescriptorSetLayout();

		void CreateLayout(VkDevice& logicalDevice) const;
		void DisposeLayout(VkDevice& logicalDevice) const;

		void CreateDescriptorPool(VkDevice& logicalDevice, SwapChainData& swapChainData, VkDescriptorPool& descriptorPool) const;
		void ResetDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
		void DisposeDescriptorPool(VkDevice& logicalDevice, VkDescriptorPool& descriptorPool) const;
		void AllocateDescriptorSet(VkDevice& logicalDevice, SwapChainData& swapChainData, 
			VkDescriptorSetLayout& descriptorSetLayout, VkImageView& textureImageView, VkSampler& textureSampler);
		void UpdateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSet& descriptorSet, 
			VkBuffer& descriptorBuffer, VkImageView& textureImageView, VkSampler& textureSampler) const;

	private:
		const uint32_t maxDescriptorSets = 4096;

		uint32_t currentSetIndex = 0;
		uint32_t currentPoolIndex = 0;

		VkDescriptorSetLayout& descriptorSetLayout;
		std::vector<VkDescriptorPool> descriptorPools;
		std::vector<VkDescriptorSet> descriptorSets;

		VkDescriptorPool& GetFreePool(VkDevice& logicalDevice, SwapChainData& swapChainData);
	};
}
