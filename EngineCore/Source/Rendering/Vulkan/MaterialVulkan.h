#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "TextureVulkan.h"
#include "GraphicsApiVulkan.h"

#include "Entities/Material.h"

#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/Models/BufferModel.h"
#include "Rendering/Vulkan/Builders/AUniformBufferVulkan.h"

namespace AVulkan
{
	class MaterialVulkan : public Material
	{
	public:
		MaterialVulkan(std::string pipelineId, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<Descriptors> descriptors,
			VkSampler& textureSampler, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback);
		~MaterialVulkan();

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