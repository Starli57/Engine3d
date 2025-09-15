#pragma once

#include <GLFW/glfw3.h>
#include <thread>
#include <mutex>

#include "VulkanContext.h"
#include "EngineCore/Core/Ref.h"

namespace VulkanApi
{
    class CommandsManager
    {
    public:
        CommandsManager(VulkanContext* vulkanContext, const int buffersCount);
        ~CommandsManager();

        void CreateCommandPool();
        VkCommandPool& GetCommandPool();
        VkCommandBuffer& GetCommandBuffer(int frame);
        
    private:
        VulkanContext* vulkanContext;
        const int buffersCount;
        
        std::unordered_map<std::thread::id, uint32_t> threadsMapping;
        std::vector<VkCommandPool> commandPools;
        std::vector<std::vector<VkCommandBuffer>> commandBuffers;
        
        URef<std::mutex> createPoolMutex;
    };
}
