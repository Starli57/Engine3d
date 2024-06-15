#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"

namespace AVulkan
{
	class Descriptors
	{
	public:
		VkDescriptorSetLayout& GetDescriptorSetLayout();

		void CreateLayout(VkDevice& logicalDevice);
		void DisposeLayout(VkDevice& logicalDevice) const;

		VkDescriptorPool& CreateDescriptorPool(VkDevice& logicalDevice, Ref<Rollback> rollback);

		VkDescriptorSet AllocateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback);
		void UpdateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSet& descriptorSet, 
			VkBuffer& descriptorBuffer, VkImageView& textureImageView, VkSampler& textureSampler, VkDeviceSize&& range) const;

	private:
		const uint32_t maxDescriptorSets = 4096;

		uint32_t currentSetIndex = 0;
		uint32_t currentPoolIndex = 0;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorPool> descriptorPools;

		VkDescriptorPool& GetFreePool(VkDevice& logicalDevice, Ref<Rollback> rollback);
	};
}
