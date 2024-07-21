#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "TextureVulkan.h"
#include "VulkanGraphicsApi.h"

#include "Entities/Material.h"

#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/Models/BufferModel.h"
#include "Rendering/Vulkan/Builders/AUniformBufferVulkan.h"

namespace AVulkan
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(std::string pipelineId, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<Descriptors> descriptors,
			VkSampler& textureSampler, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback);
		~VulkanMaterial();

		void UpdateDescriptors(uint16_t frame);

		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<Ref<BufferModel>> uboViewProjection;
		std::vector<Ref<BufferModel>> uboLights;

	private:
		Ref<Descriptors> descriptors;

		VkDevice& logicalDevice;
		VkSampler& textureSampler;
	};
}