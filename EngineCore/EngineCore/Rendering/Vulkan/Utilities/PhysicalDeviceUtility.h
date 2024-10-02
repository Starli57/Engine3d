#pragma once

#include <GLFW/glfw3.h>
#include <optional>

#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "EngineCore/Rendering/Vulkan/SwapChain.h"

namespace VkUtils
{
	std::vector<VkPhysicalDevice> GetDevicesList(VkInstance& instance);
	std::vector<VkPhysicalDevice> GetRenderingDevicesList(VkInstance& instance, VkSurfaceKHR& surface);

	VkPhysicalDevice GetBestRenderingDevice(VkInstance& instance, VkSurfaceKHR& surface);
	AVulkan::QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface);

	void PrintPhysicalDeviceDebugInformation(VkPhysicalDevice& device, VkSurfaceKHR& surface);

	uint64_t CalculateRenderingScore(VkPhysicalDevice& device);

	bool DoSupportQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface);
	bool DoSupportPhysicalDeviceExtensions(VkPhysicalDevice& device);

	VkSampleCountFlagBits GetMaxUsableSampleCount(VkPhysicalDevice& physicalDevice);
}