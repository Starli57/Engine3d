#include "EngineCore/Pch.h"
#include "AImageView.h"

namespace AVulkan
{
	void AImageView::Create(VkDevice& logicalDevice, VkFormat& imageFormat, VkImageAspectFlags imageAspectFlags,
		VkImage& image, VkImageView& imageView, Ref<Rollback> rollback)
	{
		spdlog::info("Create image view");

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = imageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = imageAspectFlags;

		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		auto createStatus = vkCreateImageView(logicalDevice, &createInfo, nullptr, &imageView);
		CAssert::Check(createStatus == VK_SUCCESS, "Image view can't be created, status: " + createStatus);

		rollback->Add([this, logicalDevice, imageView] { vkDestroyImageView(logicalDevice, imageView, nullptr); });
	}
}