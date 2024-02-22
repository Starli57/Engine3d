#include "Pch.h"
#include "AImageView.h"
#include "Rendering/Vulkan/Utilities/VkImageViewUtility.h"

namespace AVulkan
{
	//todo: no reason to have this class specific only for swap chain, need to replace it inside VkImageViewUtility
	void AImageView::Create(VkDevice& logicalDevice, SwapChainData& swapChainData) const
	{
		spdlog::info("Create swap chain image view");

		swapChainData.imageViews.resize(swapChainData.images.size());

		for (size_t i = 0; i < swapChainData.images.size(); i++)
		{
			VkImageViewUtility::Create(logicalDevice, swapChainData.imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 
				swapChainData.images.at(i), swapChainData.imageViews.at(i));
		}
	}

	void AImageView::Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const
	{
		spdlog::info("Dispose swap chain image viewes");
		for (auto imageView : swapChainData.imageViews)
		{
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}
		swapChainData.imageViews.clear();
	}
}