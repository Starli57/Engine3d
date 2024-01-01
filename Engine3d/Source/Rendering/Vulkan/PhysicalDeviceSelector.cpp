#include "Pch.h"
#include "PhysicalDeviceSelector.h"


VkPhysicalDevice PhysicalDeviceSelector::GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface)
{
    auto devices = GetDevicesList(instance);
    
    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    int bestScore = INT16_MIN;

    for (const auto& device : devices) 
    {
        auto score = CalculateRenderingScore(device);

        if (score < bestScore) continue;
        if (!GetQueueFamilies(device, surface).isComplete())  continue;

        bestScore = score;
        bestDevice = device;
    }

    return bestDevice;
}

std::vector<VkPhysicalDevice> PhysicalDeviceSelector::GetDevicesList(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) throw std::runtime_error("Physical device not found");
    else std::cout << "Physical device found: " << deviceCount << std::endl;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    return devices;
}


QueueFamilyIndices PhysicalDeviceSelector::GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for(int i = 0; i < queueFamilies.size(); i++)
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

int PhysicalDeviceSelector::CalculateRenderingScore(VkPhysicalDevice device) 
{
    //todo: add make score calculation

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

    int totalMemory = 0;
    std::cout << "Device: " << deviceProperties.deviceName << std::endl;

    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i) 
    {
        auto memoryMb = memoryProperties.memoryHeaps[i].size / (1024 * 1024);
        totalMemory += memoryMb;

        std::cout << i << ": Size=" << memoryMb << " MB" << std::endl;
    }

    int discreteMult = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2 : 1;

    return discreteMult * totalMemory;
}