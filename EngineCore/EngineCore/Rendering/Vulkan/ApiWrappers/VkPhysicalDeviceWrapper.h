#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/VkSwapchainWrapper.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	void ChooseRenderingDevice(VulkanContext* vulkanContext);

	void RefDevicesList(const VulkanContext* vulkanContext, std::vector<VkPhysicalDevice>& outDeviceList);
	void RefRenderingDevicesList(const VulkanContext* vulkanContext, std::vector<VkPhysicalDevice>& outRenderingDeviceList);

	void SetQueueFamilies(VulkanContext* vulkanContext);

	void PrintPhysicalDeviceDebugInformation(VulkanContext* vulkanContext);

	uint64_t CalculateRenderingScore(VkPhysicalDevice& physicalDevice);

	bool DoSupportQueueFamilies(const VulkanContext* vulkanContext);
	bool DoSupportPhysicalDeviceExtensions(const VulkanContext* vulkanContext);

	void SetMaxUsableSampleCount(VulkanContext* vulkanContext);

	void SetSupportedFormat(const VulkanContext* vulkanContext, const std::vector<VkFormat>& formats,
	                        VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat& outFormat);

	void SetDepthBufferFormat(VulkanContext* vulkanContext);
}
