#pragma once

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <string>
#include <filesystem>

#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Utilities/IOUtility.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Assets/Texture.h"


namespace AVulkan
{
	class TextureVulkan : public Texture
	{
	public:
		TextureVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, 
			VkCommandPool& commandPool, Ref<VulkanConfiguration> rendererConfig, const std::filesystem::path& textureFilePath);
		virtual ~TextureVulkan() override;

		Ref<ImageModel> imageModel;
	private:
		void LoadImage(const std::filesystem::path& textureFilePath);
		void CreateImageView();

		Ref<VulkanConfiguration> rendererConfig;

		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkQueue& graphicsQueue;
		VkCommandPool& commandPool;
	};
}