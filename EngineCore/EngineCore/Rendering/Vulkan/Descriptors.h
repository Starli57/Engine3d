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
		VkDescriptorSetLayout& GetDescriptorSetLayout();

		void CreateLayout(VkDevice& logicalDevice);
		void DisposeLayout(VkDevice& logicalDevice) const;

		VkDescriptorPool& CreateDescriptorPool(VkDevice& logicalDevice, Ref<Rollback> rollback);

		VkDescriptorSet AllocateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback);
		void UpdateDescriptorSet(VkDevice& logicalDevice, VkDescriptorSet& descriptorSet,
			VkBuffer& viewProjectionDescriptorBuffer, VkDeviceSize&& viewProjectionDescriptorRange,
			VkBuffer& lightDescriptorBuffer, VkDeviceSize&& lightDescriptorRange,
			VkBuffer& cameraDescriptorBuffer, VkDeviceSize&& cameraDescriptorRange,
			VkImageView& textureImageView, VkSampler& textureSampler) const;

	private:
		const uint32_t maxDescriptorSets = 4096;

		uint32_t currentSetIndex = 0;
		uint32_t currentPoolIndex = 0;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorPool> descriptorPools;

		VkDescriptorPool& GetFreePool(VkDevice& logicalDevice, Ref<Rollback> rollback);
	};
}
