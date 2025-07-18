#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"

namespace VkUtils
{
    void CreateVkSemaphore(VkDevice logicalDevice, const VkSemaphoreCreateInfo* createInfo, 
        const VkAllocationCallbacks* allocator, VkSemaphore* semaphore, const Ref<EngineCore::Rollback>& rollback);

    void CreateVkFence(VkDevice logicalDevice, const VkFenceCreateInfo* createInfo,
        const VkAllocationCallbacks* allocator, VkFence* fence, const Ref<EngineCore::Rollback>& rollback);
}