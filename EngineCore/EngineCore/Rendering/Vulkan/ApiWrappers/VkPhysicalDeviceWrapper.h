#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"

namespace VulkanApi
{
	std::vector<VkPhysicalDevice> GetDevicesList(const VkInstance& instance);
	std::vector<VkPhysicalDevice> GetRenderingDevicesList(const VkInstance& instance, VkSurfaceKHR surface);

	void ChooseRenderingDevice(VulkanContext* vulkanContext);
	QueueFamilyIndices GetQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

	void PrintPhysicalDeviceDebugInformation(VkPhysicalDevice& device, VkSurfaceKHR& surface);

	uint64_t CalculateRenderingScore(const VkPhysicalDevice& device);

	bool DoSupportQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
	bool DoSupportPhysicalDeviceExtensions(const VkPhysicalDevice& device);

	VkSampleCountFlagBits GetMaxUsableSampleCount(const VkPhysicalDevice& physicalDevice);

	VkFormat GetSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& formats,
							 VkImageTiling tiling, VkFormatFeatureFlags feature);

	VkFormat GetDepthBufferFormat(const VkPhysicalDevice& physicalDevice);
}