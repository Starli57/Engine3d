#include "Pch.h"
#include "PhysicalDeviceInterface.h"
#include "PhysicalDeviceExtensions.h"

PhysicalDeviceInterface::PhysicalDeviceInterface()
{
    swapChainInterface = new SwapChainInterface();
}

PhysicalDeviceInterface::~PhysicalDeviceInterface()
{
    delete swapChainInterface;
}

VkPhysicalDevice PhysicalDeviceInterface::GetBestRenderingDevice(VkInstance instance, VkSurfaceKHR surface)
{
    auto devices = GetRenderingDevicesList(instance, surface);

    if (devices.size() == 0) throw std::runtime_error("Physical rendering device not found");
    else std::cout << "Physical rendering devices found: " << devices.size() << std::endl;
    
    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    uint64_t bestScore = 0;

    for (const auto& device : devices) 
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

std::vector<VkPhysicalDevice> PhysicalDeviceInterface::GetDevicesList(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    return devices;
}

std::vector<VkPhysicalDevice> PhysicalDeviceInterface::GetRenderingDevicesList(VkInstance instance, VkSurfaceKHR surface)
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


QueueFamilyIndices PhysicalDeviceInterface::GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
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

uint64_t PhysicalDeviceInterface::CalculateRenderingScore(VkPhysicalDevice device)
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

bool PhysicalDeviceInterface::DoSupportQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return GetQueueFamilies(device, surface).isComplete();
}

bool PhysicalDeviceInterface::DoSupportPhysicalDeviceExtensions(VkPhysicalDevice device)
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

bool PhysicalDeviceInterface::DoSupportSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return swapChainInterface->DoSupportSwapChain(device, surface);
}
