#include "EngineCore/Pch.h"
#include "LogicalDeviceUtility.h"
#include "PhysicalDeviceUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ValidationLayersUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/PhysicalDeviceExtensions.h"

namespace VkUtils
{
	VkDevice CreateLogicalDevice(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface, VkQueue& graphicsQueue, VkQueue& presentationQueue)
	{
		spdlog::info("Create logical device");

		auto queueFamilies = GetQueueFamilies(physicalDevice, windowSurface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies =
		{
			queueFamilies.graphicsFamily.value(),
			queueFamilies.presentationFamily.value()
		};

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;

			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(AVulkan::physicalDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = AVulkan::physicalDeviceExtensions.data();

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		createInfo.pEnabledFeatures = &deviceFeatures;

		VkUtils::SetupValidationLayers(createInfo);

		VkDevice logicalDevice;
		auto createStatus = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
		CAssert::Check(createStatus == VK_SUCCESS, "failed to create logical device, status: " + createStatus);

		vkGetDeviceQueue(logicalDevice, queueFamilies.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(logicalDevice, queueFamilies.presentationFamily.value(), 0, &presentationQueue);

		return logicalDevice;
	}

	void DisposeLogicalDevice(const VkDevice& logicalDevice)
	{
		spdlog::info("Dispose logical device");
		vkDestroyDevice(logicalDevice, nullptr);
	}
}