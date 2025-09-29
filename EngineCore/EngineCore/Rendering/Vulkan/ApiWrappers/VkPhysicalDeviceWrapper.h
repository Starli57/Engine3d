#pragma once

#include <GLFW/glfw3.h>
#include <optional>

#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "EngineCore/Rendering/Vulkan/SwapchainManager.h"

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

	void SetSupportedFormat(const VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& formats,
							 VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat& outFormat);

	void SetDepthBufferFormat(const VkPhysicalDevice& physicalDevice, VkFormat& outFormat);
}