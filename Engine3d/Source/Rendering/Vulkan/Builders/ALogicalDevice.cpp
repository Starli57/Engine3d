#include "Pch.h"
#include "ALogicalDevice.h"
#include "APhysicalDevice.h"
#include "AValidationLayers.h"
#include "Rendering/Vulkan/Models/PhysicalDeviceExtensions.h"

namespace AVulkan
{
	void ALogicalDevice::Create(Ref<VulkanModel> vulkanModel) const
	{
		spdlog::info("Create logical device");

		APhysicalDevice APhysicalDevice;
		AValidationLayers validationLayers;

		auto queueFamilies = APhysicalDevice.GetQueueFamilies(vulkanModel->physicalDevice, vulkanModel->windowSurface);

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
		vkGetPhysicalDeviceFeatures(vulkanModel->physicalDevice, &deviceFeatures);
		createInfo.pEnabledFeatures = &deviceFeatures;

		validationLayers.Setup(createInfo);

		VkDevice logicalDevice;
		auto createStatus = vkCreateDevice(vulkanModel->physicalDevice, &createInfo, nullptr, &logicalDevice);
		CAssert::Check(createStatus == VK_SUCCESS, "failed to create logical device, status: " + createStatus);

		vkGetDeviceQueue(logicalDevice, queueFamilies.graphicsFamily.value(), 0, &vulkanModel->graphicsQueue);
		vkGetDeviceQueue(logicalDevice, queueFamilies.presentationFamily.value(), 0, &vulkanModel->presentationQueue);

		vulkanModel->logicalDevice = logicalDevice;
	}

	void ALogicalDevice::Dispose(VkDevice& logicalDevice) const
	{
		spdlog::info("Dispose logical device");
		vkDestroyDevice(logicalDevice, nullptr);
	}
}