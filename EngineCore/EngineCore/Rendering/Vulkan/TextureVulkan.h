#pragma once

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <string>
#include <filesystem>

#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Builders/AUniformBufferVulkan.h"
#include "EngineCore/Rendering/Vulkan/Builders/AImage.h"
#include "EngineCore/Rendering/Vulkan/Builders/AImageView.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/ProjectSettings.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/IOUtility.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Texture.h"


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