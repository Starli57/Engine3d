#include "EngineCore/Pch.h"
#include "TextureSamplerUtility.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/EngineApi.h"

namespace VkUtils
{
    void CreateTextureSampler(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, VkSampler& textureSampler)
    {
        VkPhysicalDeviceProperties2 properties{};
        properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vkGetPhysicalDeviceProperties2(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.properties.limits.maxSamplerAnisotropy;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;

        const auto createStatus = vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &textureSampler);
        Engine::CAssert::Check(createStatus == VK_SUCCESS, "Textures sampler can't be created");
    }
}
