#include "Pch.h"
#include "AImageView.h"

namespace AVulkan
{
	void AImageView::Create(VkDevice& logicalDevice, SwapChainData& swapChainData) const
	{
		spdlog::info("Create swap chain image view");

		swapChainData.imageViews.resize(swapChainData.images.size());

		for (size_t i = 0; i < swapChainData.images.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainData.images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainData.imageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			auto createStatus = vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainData.imageViews[i]);
			CAssert::Check(createStatus == VK_SUCCESS, "Swap chain image view can't be created, status: " + createStatus);
		}
	}

	void AImageView::Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const
	{
		spdlog::info("Dispose swap chain image viewes");
		for (auto imageView : swapChainData.imageViews)
		{
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}
	}
}