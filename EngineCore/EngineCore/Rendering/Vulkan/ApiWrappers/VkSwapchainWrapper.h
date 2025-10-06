#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
    void RecreateSwapchain(VulkanContext* vulkanContext, GLFWwindow* window);
    void CreateSwapchain(VulkanContext* vulkanContext, GLFWwindow* window);
    void CreateSwapChainImageViews(VulkanContext* vulkanContext);
    void CreateDepthBuffer(VulkanContext* vulkanContext);
    void CreateMSAAColorResources(VulkanContext* vulkanContext);
    void CreateMSAADepthResources(VulkanContext* vulkanContext);
    void DisposeSwapchain(VulkanContext* vulkanContext);
	
    VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities);

    void SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
        VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities,
        QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount);
}
