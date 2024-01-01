#include "Pch.h"
#include "PhysicalDeviceSelector.h"
#include "PhysicalDeviceExtensions.h"

PhysicalDeviceSelector::PhysicalDeviceSelector() 
{
    swapChainInterface = new SwapChainInterface();
}

PhysicalDeviceSelector::~PhysicalDeviceSelector()
{
    delete swapChainInterface;
}

VkPhysicalDevice PhysicalDeviceSelector::GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface)
{
    auto devices = GetDevicesList(instance);
    
    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    uint64_t bestScore = 0;

    for (const auto& device : devices) 
    {
        auto score = CalculateRenderingScore(device);

        if (score < bestScore) continue;
        if (!DoSupportQueueFamilies(device, surface))  continue;
        if (!DoSupportPhysicalDeviceExtensions(device)) continue;
        if (!DoSupportSwapChain(device, surface)) continue;

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

uint64_t PhysicalDeviceSelector::CalculateRenderingScore(VkPhysicalDevice device)
{
    //todo: make better score calculation

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

    uint64_t totalMemory = 0;
    std::cout << "Device: " << deviceProperties.deviceName << std::endl;

    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i) 
    {
        auto memoryMb = memoryProperties.memoryHeaps[i].size / (1024 * 1024);
        totalMemory += memoryMb;

        std::cout << i << ": Size=" << memoryMb << " MB" << std::endl;
    }

    uint64_t discreteMult = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2 : 1;

    return discreteMult * totalMemory;
}

bool PhysicalDeviceSelector::DoSupportQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return GetQueueFamilies(device, surface).isComplete();
}

bool PhysicalDeviceSelector::DoSupportPhysicalDeviceExtensions(VkPhysicalDevice device)
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

bool PhysicalDeviceSelector::DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return swapChainInterface->DoSupportSwapChain(device, surface);
}
