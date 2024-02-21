#include "Pch.h"
#include "VkFormatUtility.h"

namespace AVulkan
{
    VkFormat VkFormatUtility::FindSupportedFormat(VkPhysicalDevice& physicalDevice, std::vector<VkFormat>& formats,
        VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (VkFormat format : formats) 
        {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

            if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) 
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) 
            {
                return format;
            }
        }

        throw std::runtime_error("Failed to find supported format!");
    }

    VkFormat VkFormatUtility::FindDepthBufferFormat(VkPhysicalDevice& physicalDevice)
    {
        std::vector<VkFormat> formats =
        {
            VK_FORMAT_D32_SFLOAT, 
            VK_FORMAT_D32_SFLOAT_S8_UINT, 
            VK_FORMAT_D24_UNORM_S8_UINT 
        };

        return FindSupportedFormat(physicalDevice, formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
}