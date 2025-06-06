#pragma once

#include <GLFW/glfw3.h>
#include <optional>

#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "EngineCore/Rendering/Vulkan/SwapChain.h"

namespace VkUtils
{
	std::vector<VkPhysicalDevice> GetDevicesList(const VkInstance& instance);
	std::vector<VkPhysicalDevice> GetRenderingDevicesList(const VkInstance& instance, VkSurfaceKHR& surface);

	VkPhysicalDevice GetBestRenderingDevice(VkInstance& instance, VkSurfaceKHR& surface);
	AVulkan::QueueFamilyIndices GetQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

	void PrintPhysicalDeviceDebugInformation(VkPhysicalDevice& device, VkSurfaceKHR& surface);

	uint64_t CalculateRenderingScore(const VkPhysicalDevice& device);

	bool DoSupportQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
	bool DoSupportPhysicalDeviceExtensions(const VkPhysicalDevice& device);

	VkSampleCountFlagBits GetMaxUsableSampleCount(const VkPhysicalDevice& physicalDevice);
}