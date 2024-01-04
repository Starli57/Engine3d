#include "Pch.h"
#include "LogicalDeviceInterface.h"
#include "PhysicalDeviceInterface.h"
#include "ValidationLayersInterface.h"
#include "PhysicalDeviceExtensions.h"

VkDevice LogicalDeviceInterface::Create(VkPhysicalDevice physicalDevice, VkSurfaceKHR& windowSurface,
	VkQueue& graphicsQueue, VkQueue& presentationQueue)
{
	PhysicalDeviceInterface physicalDeviceInterface;
	ValidationLayersInterface validationLayers;

	auto queueFamilies = physicalDeviceInterface.GetQueueFamilies(physicalDevice, windowSurface);

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
	createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = physicalDeviceExtensions.data();

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	createInfo.pEnabledFeatures = &deviceFeatures;

	validationLayers.Setup(createInfo);

	VkDevice logicalDevice;
	auto createStatus = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
	if (createStatus != VK_SUCCESS) {
		std::cout << "failed to create logical device, status: " << createStatus;//todo: add errors handling
	}

	vkGetDeviceQueue(logicalDevice, queueFamilies.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, queueFamilies.presentationFamily.value(), 0, &presentationQueue);

	return logicalDevice;
}
