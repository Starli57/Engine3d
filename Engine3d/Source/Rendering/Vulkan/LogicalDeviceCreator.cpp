#include "Pch.h"
#include "LogicalDeviceCreator.h"
#include "PhysicalDeviceSelector.h"
#include "ValidationLayers.h"

VkDevice LogicalDeviceCreator::Create(VkPhysicalDevice physicalDevice)
{
	PhysicalDeviceSelector physicalDeviceSelector;
	ValidationLayers validationLayers;

	auto queueFamily = physicalDeviceSelector.GetQueueFamilies(physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamily.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

	validationLayers.Setup(createInfo);

	VkDevice logicalDevice;
	auto createStatus = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
	if (createStatus != VK_SUCCESS) {
		std::cout << "failed to create logical device, status: " << createStatus;//todo: add errors handling
	}

	VkQueue graphicsQueue;
	vkGetDeviceQueue(logicalDevice, queueFamily.graphicsFamily.value(), 0, &graphicsQueue);

	return logicalDevice;
}
