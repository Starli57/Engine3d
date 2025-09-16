#include "EngineCore/Pch.h"
#include "CommandPoolUtility.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/Utilities/PhysicalDeviceUtility.h"
#include "spdlog/spdlog.h"

namespace VulkanApi
{
	VkCommandPool CreateCommandPool(const VulkanContext* vulkanContext)
	{
		spdlog::info("Create command pool");
		QueueFamilyIndices queueFamilyIndices = GetQueueFamilies(vulkanContext->physicalDevice, vulkanContext->windowSurface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		VkCommandPool commandPool;
		auto createStatus = vkCreateCommandPool(vulkanContext->logicalDevice, &poolInfo, nullptr, &commandPool);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create command pool, status = " + createStatus);

		return commandPool;
	}

	void DisposeCommandPool(const VulkanContext* vulkanContext, const VkCommandPool& commandPool)
	{
		spdlog::info("Dispose command pool");
		vkDestroyCommandPool(vulkanContext->logicalDevice, commandPool, nullptr);
	}
}