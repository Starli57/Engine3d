#include "Pch.h"
#include "LogicalDeviceCreator.h"
#include "PhysicalDeviceSelector.h"
#include "ValidationLayers.h"
#include "PhysicalDeviceExtensions.h"

VkDevice LogicalDeviceCreator::Create(VkPhysicalDevice physicalDevice, VkSurfaceKHR& windowSurface,
	VkQueue& graphicsQueue, VkQueue& presentationQueue)
{
	PhysicalDeviceSelector physicalDeviceSelector;
	ValidationLayers validationLayers;

	auto queueFamilies = physicalDeviceSelector.GetQueueFamilies(physicalDevice, windowSurface);

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
