#include "Pch.h"
#include "CommandPoolUtility.h"
#include "Rendering/Vulkan/Builders/APhysicalDevice.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
	VkCommandPool CreateCommandPool(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface)
	{
		spdlog::info("Create command pool");
		AVulkan::QueueFamilyIndices queueFamilyIndices = AVulkan::APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		VkCommandPool commandPool;
		auto createStatus = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create command pool, status = " + createStatus);

		return commandPool;
	}

	void DisposeCommandPool(VkDevice& logicalDevice, VkCommandPool& commandPool)
	{
		spdlog::info("Dispose command pool");
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	}
}