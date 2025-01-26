#include "EngineCore/Pch.h"
#include "CommandPoolUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/PhysicalDeviceUtility.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
	VkCommandPool CreateCommandPool(const VkDevice& logicalDevice, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& windowSurface)
	{
		spdlog::info("Create command pool");
		AVulkan::QueueFamilyIndices queueFamilyIndices = GetQueueFamilies(physicalDevice, windowSurface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		VkCommandPool commandPool;
		auto createStatus = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create command pool, status = " + createStatus);

		return commandPool;
	}

	void DisposeCommandPool(const VkDevice& logicalDevice, const VkCommandPool& commandPool)
	{
		spdlog::info("Dispose command pool");
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	}
}