#include "EngineCore/Pch.h"
#include "FormatUtility.h"

namespace VulkanApi
{
    VkFormat FindSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& formats,
                                 const VkImageTiling tiling, const VkFormatFeatureFlags features)
    {
        for (const VkFormat format : formats) 
        {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

            if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
                return format;
            
            if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) 
                return format;
        }

        throw std::runtime_error("Failed to find supported format!");
    }

    VkFormat FindDepthBufferFormat(const VkPhysicalDevice& physicalDevice)
    {
        const std::vector<VkFormat> formats =
        {
            VK_FORMAT_D32_SFLOAT, 
            VK_FORMAT_D32_SFLOAT_S8_UINT, 
            VK_FORMAT_D24_UNORM_S8_UINT 
        };

        return FindSupportedFormat(physicalDevice, formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
}