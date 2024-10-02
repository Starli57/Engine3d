#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "EngineCore/Rendering/Vulkan/Models/ImageModel.h"
#include "EngineCore/Core/Ref.h"

namespace AVulkan
{
	struct SwapChainData
	{
	public:

		VkSwapchainKHR swapChain;
		VkSwapchainKHR oldSwapChain;//todo: add usage to reuse resources

		VkSurfaceFormatKHR surfaceFormat;
		VkExtent2D extent;

		uint32_t imagesCount;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		std::vector<VkFramebuffer> frameBuffers;

		Ref<ImageModel> depthBufferModel;
		Ref<ImageModel> msaaColorSample;
		Ref<ImageModel> msaaDepthSample;
	};
}