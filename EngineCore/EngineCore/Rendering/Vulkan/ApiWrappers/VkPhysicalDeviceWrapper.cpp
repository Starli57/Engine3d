#include "EngineCore/Pch.h"
#include "VkPhysicalDeviceWrapper.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/Models/PhysicalDeviceExtensions.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapchainSurfaceSettings.h"

namespace VulkanApi
{
    void ChooseRenderingDevice(VulkanContext* vulkanContext)
    {
        spdlog::info("Select physical rendering device");

        std::vector<VkPhysicalDevice> renderingDevices;
        RefRenderingDevicesList(vulkanContext, renderingDevices);

        Engine::CAssert::Check(!renderingDevices.empty(), "Physical rendering device not found");
        spdlog::info("Physical rendering devices found: {0}", renderingDevices.size());

        uint64_t bestScore = 0;

        for (auto& device : renderingDevices)
        {
            const auto score = CalculateRenderingScore(device);
            if (score > bestScore)
            {
                bestScore = score;
                vulkanContext->physicalDevice = device;
            }
        }
    }

    void RefDevicesList(const VulkanContext* vulkanContext, std::vector<VkPhysicalDevice>& outDevices)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkanContext->instance, &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices;
        devices.resize(deviceCount);
        vkEnumeratePhysicalDevices(vulkanContext->instance, &deviceCount, devices.data());
        
        outDevices.resize(deviceCount);
        for (uint32_t deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex) outDevices[deviceIndex] = devices[0];
    }

    void RefRenderingDevicesList(const VulkanContext* vulkanContext, std::vector<VkPhysicalDevice>& outRenderingDevices)
    {
        RefDevicesList(vulkanContext, outRenderingDevices);

        for (int i = 0; i < outRenderingDevices.size(); i++)
        {
            auto device = outRenderingDevices.at(i);
            if (!DoSupportQueueFamilies(vulkanContext))  continue;
            if (!DoSupportPhysicalDeviceExtensions(vulkanContext)) continue;
            if (!DoSupportSwapchain(vulkanContext)) continue;
            outRenderingDevices.push_back(device);
        }
    }
    
    void SetQueueFamilies(VulkanContext* vulkanContext)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vulkanContext->physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vulkanContext->physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilies.size(); i++)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vulkanContext->physicalDevice, i, vulkanContext->windowSurface, &presentSupport);

            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) vulkanContext->queueFamilies.graphicsFamily = i;
            if (presentSupport) vulkanContext->queueFamilies.presentationFamily = i;
        }
    }

    uint64_t CalculateRenderingScore(VkPhysicalDevice& physicalDevice)
    {
        //todo: make better score calculation

        VkPhysicalDeviceProperties2 deviceProperties {};
        deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vkGetPhysicalDeviceProperties2(physicalDevice, &deviceProperties);
        
        VkPhysicalDeviceMemoryProperties2 memoryProperties {};
        memoryProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
        vkGetPhysicalDeviceMemoryProperties2(physicalDevice, &memoryProperties);

        uint64_t totalMemory = 0;
        spdlog::info("Device: {0}", deviceProperties.properties.deviceName);

        for (uint32_t i = 0; i < memoryProperties.memoryProperties.memoryHeapCount; ++i)
        {
            auto memoryMb = memoryProperties.memoryProperties.memoryHeaps[i].size / (1024 * 1024);
            totalMemory += memoryMb;

            spdlog::info("{0}: Size={1} MB", i, memoryMb);
        }

        uint64_t discreteMult = deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2 : 1;

        return discreteMult * totalMemory;
    }

    bool DoSupportQueueFamilies(const VulkanContext* vulkanContext)
    {
        return vulkanContext->queueFamilies.IsComplete();
    }

    bool DoSupportPhysicalDeviceExtensions(const VulkanContext* vulkanContext)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(vulkanContext->physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(vulkanContext->physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(physicalDeviceExtensions.begin(), physicalDeviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void PrintPhysicalDeviceDebugInformation(VulkanContext* vulkanContext)
    {
        VkPhysicalDeviceProperties2 deviceProperties {};
        deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vkGetPhysicalDeviceProperties2(vulkanContext->physicalDevice, &deviceProperties);
        spdlog::info("Rendering GPU: {0}", deviceProperties.properties.deviceName);
        
        uint32_t major = VK_API_VERSION_MAJOR(deviceProperties.properties.apiVersion);
        uint32_t minor = VK_API_VERSION_MINOR(deviceProperties.properties.apiVersion);
        spdlog::info("Device API Versions: {}.{}", major, minor);
        
        SwapchainSurfaceSettings surfaceSettings;
        SetSwapchainColorFormats(vulkanContext, surfaceSettings.formats);
        SetSwapchainPresentModes(vulkanContext, surfaceSettings.presentModes);

        for (auto colorFormat : surfaceSettings.formats)
        {
            spdlog::info("Available color format: {0} color space: {1}", static_cast<int>(colorFormat.format), static_cast<int>(colorFormat.colorSpace));
        }

        for (auto mode : surfaceSettings.presentModes)
        {
            spdlog::info("Available present mode: {0}", static_cast<int>(mode));
        }
        
        SetMaxUsableSampleCount(vulkanContext);
        spdlog::info("Max MSAA: {0}", static_cast<int>(vulkanContext->msaa));
    }

    void SetMaxUsableSampleCount(VulkanContext* vulkanContext) 
    {
        VkPhysicalDeviceProperties2 deviceProperties {};
        deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vkGetPhysicalDeviceProperties2(vulkanContext->physicalDevice, &deviceProperties);

        VkSampleCountFlags counts = deviceProperties.properties.limits.framebufferColorSampleCounts & deviceProperties.properties.limits.framebufferDepthSampleCounts;
        VkSampleCountFlagBits msaa = VK_SAMPLE_COUNT_1_BIT;
        
        if (counts & VK_SAMPLE_COUNT_64_BIT)        msaa = VK_SAMPLE_COUNT_64_BIT;
        else if (counts & VK_SAMPLE_COUNT_32_BIT)   msaa = VK_SAMPLE_COUNT_32_BIT; 
        else if (counts & VK_SAMPLE_COUNT_16_BIT)   msaa = VK_SAMPLE_COUNT_16_BIT;
        else if (counts & VK_SAMPLE_COUNT_8_BIT)    msaa = VK_SAMPLE_COUNT_8_BIT; 
        else if (counts & VK_SAMPLE_COUNT_4_BIT)    msaa = VK_SAMPLE_COUNT_4_BIT; 
        else if (counts & VK_SAMPLE_COUNT_2_BIT)    msaa = VK_SAMPLE_COUNT_2_BIT; 

        vulkanContext->msaa = msaa;
    }

    void SetSupportedFormat(const VulkanContext* vulkanContext, const std::vector<VkFormat>& formats,
            const VkImageTiling tiling, const VkFormatFeatureFlags features, VkFormat& outFormat)
    {
        for (const VkFormat format : formats) 
        {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(vulkanContext->physicalDevice, format, &properties);

            if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
            {
                outFormat = format;
                return;
            }
            
            if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
            {
                outFormat = format;
                return;
            }
        }

        throw std::runtime_error("Failed to find supported format!");
    }

    void SetDepthBufferFormat(VulkanContext* vulkanContext)
    {
        const std::vector<VkFormat> formats =
        {
            VK_FORMAT_D32_SFLOAT, 
            VK_FORMAT_D32_SFLOAT_S8_UINT, 
            VK_FORMAT_D24_UNORM_S8_UINT 
        };

        SetSupportedFormat(vulkanContext, formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, vulkanContext->depthFormat);
    }
}
