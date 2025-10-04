#include "EngineCore/Pch.h"
#include "VkCommandPoolWrapper.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkPhysicalDeviceWrapper.h"
#include "spdlog/spdlog.h"

namespace VulkanApi
{
	void CreateCommandPool(const VulkanContext* vulkanContext, VkCommandPool& outCommandPool)
	{
		spdlog::info("Create command pool");
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = vulkanContext->queueFamilies.graphicsFamily.value();

		auto createStatus = vkCreateCommandPool(vulkanContext->logicalDevice, &poolInfo, nullptr, &outCommandPool);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create command pool, status = " + createStatus);
	}

	void DisposeCommandPool(const VulkanContext* vulkanContext, const VkCommandPool& commandPool)
	{
		spdlog::info("Dispose command pool");
		vkDestroyCommandPool(vulkanContext->logicalDevice, commandPool, nullptr);
	}
}