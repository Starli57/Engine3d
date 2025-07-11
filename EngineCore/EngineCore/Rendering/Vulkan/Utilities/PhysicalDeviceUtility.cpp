#include "EngineCore/Pch.h"
#include "PhysicalDeviceUtility.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/Models/PhysicalDeviceExtensions.h"
#include "EngineCore/Rendering/Vulkan/Utilities/SwapchainUtility.h"

namespace VkUtils
{
    VkPhysicalDevice GetBestRenderingDevice(VkInstance& instance, VkSurfaceKHR& surface)
    {
        spdlog::info("Select physical rendering device");

        auto devices = GetRenderingDevicesList(instance, surface);

        CAssert::Check(!devices.empty(), "Physical rendering device not found");
        spdlog::info("Physical rendering devices found: {0}", devices.size());

        VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
        uint64_t bestScore = 0;

        for (auto& device : devices)
        {
            const auto score = CalculateRenderingScore(device);
            if (score > bestScore)
            {
                bestScore = score;
                bestDevice = device;
            }
        }

        return bestDevice;
    }

    std::vector<VkPhysicalDevice> GetDevicesList(const VkInstance& instance)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        return devices;
    }

    std::vector<VkPhysicalDevice> GetRenderingDevicesList(const VkInstance& instance, VkSurfaceKHR& surface)
    {
        auto allDevices = GetDevicesList(instance);

        std::vector<VkPhysicalDevice> renderingDevices;
        for (int i = 0; i < allDevices.size(); i++)
        {
            auto device = allDevices.at(i);
            if (!DoSupportQueueFamilies(device, surface))  continue;
            if (!DoSupportPhysicalDeviceExtensions(device)) continue;
            if (!DoSupportSwapChain(device, surface)) continue;
            renderingDevices.push_back(device);
        }

        return renderingDevices;
    }


    AVulkan::QueueFamilyIndices GetQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
    {
        AVulkan::QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int i = 0; i < queueFamilies.size(); i++)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            if (presentSupport)
                indices.presentationFamily = i;
        }

        return indices;
    }

    uint64_t CalculateRenderingScore(const VkPhysicalDevice& device)
    {
        //todo: make better score calculation

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

        uint64_t totalMemory = 0;
        spdlog::info("Device: {0}", deviceProperties.deviceName);

        for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
        {
            auto memoryMb = memoryProperties.memoryHeaps[i].size / (1024 * 1024);
            totalMemory += memoryMb;

            spdlog::info("{0}: Size={1} MB", i, memoryMb);
        }

        uint64_t discreteMult = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2 : 1;

        return discreteMult * totalMemory;
    }

    bool DoSupportQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
    {
        return GetQueueFamilies(device, surface).IsComplete();
    }

    bool DoSupportPhysicalDeviceExtensions(const VkPhysicalDevice& device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(AVulkan::physicalDeviceExtensions.begin(), AVulkan::physicalDeviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void PrintPhysicalDeviceDebugInformation(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        spdlog::info("Rendering GPU: {0}", deviceProperties.deviceName);

        AVulkan::SwapChainSurfaceSettings surfaceSettings;

        VkUtils::GetSwapChainColorFormats(physicalDevice, windowSurface, surfaceSettings.formats);
        VkUtils::GetSwapChainPresentModes(physicalDevice, windowSurface, surfaceSettings.presentModes);

        for (auto colorFormat : surfaceSettings.formats)
        {
            spdlog::info("Available color format: {0} color space: {1}", static_cast<int>(colorFormat.format), static_cast<int>(colorFormat.colorSpace));
        }

        for (auto mode : surfaceSettings.presentModes)
        {
            spdlog::info("Available present mode: {0}", static_cast<int>(mode));
        }

        spdlog::info("Max MSAA: {0}", static_cast<int>(GetMaxUsableSampleCount(physicalDevice)));
    }

    VkSampleCountFlagBits GetMaxUsableSampleCount(const VkPhysicalDevice& physicalDevice) 
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }
}