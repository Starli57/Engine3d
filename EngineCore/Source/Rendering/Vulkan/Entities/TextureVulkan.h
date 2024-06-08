#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "Resources/TexturesList.h"
#include "Entities/Texture.h"
#include "Rendering/Vulkan/Models/ImageModel.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/Models/BufferModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineShared/ProjectSettings.h"
#include "EngineShared/IOUtility.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"

namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(Ref<ProjectSettigns> projectSettings, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
			Ref<Descriptors> descriptors, VkDescriptorSetLayout& descriptorSetLayout, 
			VkSampler& textureSampler, VkQueue& graphicsQueue, VkCommandPool& commandPool, TextureId textureId, Ref<Rollback> rollback);
		virtual ~TextureVulkan() override;

		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<Ref<BufferModel>> uniformBuffers;

	private:
		void CreateImage(TextureId textureId);
		void CreateImageView();

		Ref<Rollback> rollback;
		Ref<ProjectSettigns> projectSettings;
		Ref<ImageModel> imageModel;

		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkQueue& graphicsQueue;
		VkCommandPool& commandPool;
	};
}