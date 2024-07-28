#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "TextureVulkan.h"
#include "GraphicsApiVulkan.h"

#include "EngineCore/Assets/Material.h"
#include "EngineCore/AssetsDatabase.h"
#include "EngineCore/Rendering/Vulkan/Descriptors.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"
#include "EngineCore/Rendering/Vulkan/Builders/AUniformBufferVulkan.h"

namespace AVulkan
{
	class MaterialVulkan : public Material
	{
	public:
		MaterialVulkan(std::string pipelineId, Ref<AssetsDatabase> assetDatabase, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
			Ref<Descriptors> descriptors, VkSampler& textureSampler, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback);
		~MaterialVulkan();

		void UpdateDescriptors(uint16_t frame);

		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<Ref<BufferModel>> uboViewProjection;
		std::vector<Ref<BufferModel>> uboLights;

	private:
		Ref<AssetsDatabase> assetDatabase;
		Ref<Descriptors> descriptors;

		VkDevice& logicalDevice;
		VkSampler& textureSampler;
	};
}