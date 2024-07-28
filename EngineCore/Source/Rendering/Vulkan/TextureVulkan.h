#pragma once

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <string>
#include <filesystem>

#include "Rendering/Vulkan/GraphicsApiVulkan.h"
#include "Rendering/Vulkan/Utilities/BufferUtility.h"
#include "Rendering/Vulkan/Builders/AUniformBufferVulkan.h"
#include "Rendering/Vulkan/Builders/AImage.h"
#include "Rendering/Vulkan/Builders/AImageView.h"
#include "Rendering/Vulkan/Models/ImageModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineShared/ProjectSettings.h"
#include "EngineShared/CustomAssert.h"
#include "EngineShared/IOUtility.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"
#include "EngineShared/Components/UboDiffuseLightComponent.h"
#include "EngineShared/Texture.h"


namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, 
			VkCommandPool& commandPool, std::filesystem::path& textureFilePath, Ref<Rollback> rollback);
		virtual ~TextureVulkan() override;

		Ref<ImageModel> imageModel;
	private:
		void CreateImage(std::filesystem::path& textureFilePath, Ref<Rollback> rollback);
		void CreateImageView();

		Ref<Rollback> rollback;

		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkQueue& graphicsQueue;
		VkCommandPool& commandPool;
	};
}