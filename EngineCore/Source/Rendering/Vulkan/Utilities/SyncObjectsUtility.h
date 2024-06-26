#pragma once

#include <GLFW/glfw3.h>
#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"

namespace VkUtils
{
    void CreateVkSemaphore(VkDevice logicalDevice, const VkSemaphoreCreateInfo* createInfo, 
        const VkAllocationCallbacks* allocator, VkSemaphore* semaphore, Ref<Rollback> rollback);

    void CreateVkFence(VkDevice logicalDevice, const VkFenceCreateInfo* createInfo,
        const VkAllocationCallbacks* allocator, VkFence* fence, Ref<Rollback> rollback);
}