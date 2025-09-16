#include "EngineCore/Pch.h"
#include "SyncObjectsUtility.h"

namespace VulkanApi
{
	void CreateVkSemaphore(VkDevice logicalDevice, const VkSemaphoreCreateInfo* createInfo, const VkAllocationCallbacks* allocator,
		VkSemaphore* semaphore, const Ref<Engine::Rollback>& rollback)
	{
		vkCreateSemaphore(logicalDevice, createInfo, allocator, semaphore);
		rollback->Add([logicalDevice, semaphore]() { vkDestroySemaphore(logicalDevice, *semaphore, nullptr); });
	}

	void CreateVkFence(VkDevice logicalDevice, const VkFenceCreateInfo* createInfo, const VkAllocationCallbacks* allocator,
		VkFence* fence, const Ref<Engine::Rollback>& rollback)
	{
		vkCreateFence(logicalDevice, createInfo, nullptr, fence);
		rollback->Add([logicalDevice, fence]() { vkDestroyFence(logicalDevice, *fence, nullptr); });
	}
}