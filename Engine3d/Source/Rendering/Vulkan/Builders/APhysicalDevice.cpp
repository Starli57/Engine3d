#include "Pch.h"
#include "APhysicalDevice.h"
#include "Rendering/Vulkan/Models/PhysicalDeviceExtensions.h"
#include "Rendering/Vulkan/Utilities/VkSwapchainUtility.h"

namespace AVulkan
{
    VkPhysicalDevice APhysicalDevice::GetBestRenderingDevice(VkInstance& instance, VkSurfaceKHR& surface) const
    {
        spdlog::info("Select physical rendering device");

        auto devices = GetRenderingDevicesList(instance, surface);

        CAssert::Check(devices.size() != 0, "Physical rendering device not found");
        spdlog::info("Physical rendering devices found: {0}", devices.size());

        VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
        uint64_t bestScore = 0;

        for (auto& device : devices)
        {
            auto score = CalculateRenderingScore(device);
            if (score > bestScore)
            {
                bestScore = score;
                bestDevice = device;
            }
        }

        return bestDevice;
    }

    std::vector<VkPhysicalDevice> APhysicalDevice::GetDevicesList(VkInstance& instance) const
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        return devices;
    }

    std::vector<VkPhysicalDevice> APhysicalDevice::GetRenderingDevicesList(VkInstance& instance, VkSurfaceKHR& surface) const
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


    QueueFamilyIndices APhysicalDevice::GetQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) const
    {
        QueueFamilyIndices indices;

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

    uint64_t APhysicalDevice::CalculateRenderingScore(VkPhysicalDevice& device) const
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

    bool APhysicalDevice::DoSupportQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) const
    {
        return GetQueueFamilies(device, surface).isComplete();
    }

    bool APhysicalDevice::DoSupportPhysicalDeviceExtensions(VkPhysicalDevice& device) const
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(physicalDeviceExtensions.begin(), physicalDeviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool APhysicalDevice::DoSupportSwapChain(VkPhysicalDevice& device, VkSurfaceKHR& surface) const
    {
        return VkUtilities::DoSupportSwapChain(device, surface);
    }

    void APhysicalDevice::PrintDebugInformation(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface) const
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        spdlog::info("Rendering GPU: {0}", deviceProperties.deviceName);

        SwapChainSurfaceSettings surfaceSettings;

        VkUtilities::GetSwapChainColorFormats(physicalDevice, windowSurface, surfaceSettings.formats);
        VkUtilities::GetSwapChainPresentModes(physicalDevice, windowSurface, surfaceSettings.presentModes);

        for (auto colorFormat : surfaceSettings.formats)
        {
            spdlog::info("Available color format: {0} color space: {1}", colorFormat.format, colorFormat.colorSpace);
        }

        for (auto mode : surfaceSettings.presentModes)
        {
            spdlog::info("Available present mode: {0}", mode);
        }
    }
}