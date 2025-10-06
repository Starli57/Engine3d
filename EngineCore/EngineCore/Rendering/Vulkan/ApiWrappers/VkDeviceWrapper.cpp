#include "EngineCore/Pch.h"
#include "VkDeviceWrapper.h"
#include "VkPhysicalDeviceWrapper.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/ValidationLayersUtility.h"

namespace VulkanApi
{
	VkDevice CreateLogicalDevice(VulkanContext* vulkanContext)
	{
		spdlog::info("Create logical device");

		auto queueFamilies = GetQueueFamilies(vulkanContext->physicalDevice, vulkanContext->windowSurface);

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
		
		VkPhysicalDeviceFeatures2 deviceFeatures{};
		deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		vkGetPhysicalDeviceFeatures2(vulkanContext->physicalDevice, &deviceFeatures);

#if DEBUG
		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures{};
		bufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
		bufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay = VK_TRUE;
		
		deviceFeatures.pNext = &bufferDeviceAddressFeatures;
#endif
		
		createInfo.pNext = &deviceFeatures;

		SetupValidationLayers(createInfo);

		VkDevice logicalDevice;
		auto createStatus = vkCreateDevice(vulkanContext->physicalDevice, &createInfo, nullptr, &logicalDevice);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "failed to create logical device, status: " + createStatus);

		vkGetDeviceQueue(logicalDevice, queueFamilies.graphicsFamily.value(), 0, &vulkanContext->graphicsQueue);
		vkGetDeviceQueue(logicalDevice, queueFamilies.presentationFamily.value(), 0, &vulkanContext->presentationQueue);

		return logicalDevice;
	}

	void DisposeLogicalDevice(const VkDevice& logicalDevice)
	{
		spdlog::info("Dispose logical device");
		vkDestroyDevice(logicalDevice, nullptr);
	}
}