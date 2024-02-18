#include "Pch.h"
#include "ACommandPool.h"
#include "APhysicalDevice.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	VkCommandPool ACommandPool::Create(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& windowSurface) const
	{
		spdlog::info("Create command pool");
		QueueFamilyIndices queueFamilyIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		VkCommandPool commandPool;
		auto createStatus = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create command pool, status = " + createStatus);

		return commandPool;
	}

	void ACommandPool::Dispose(VkDevice& logicalDevice, VkCommandPool& commandPool) const
	{
		spdlog::info("Dispose command pool");
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	}
}