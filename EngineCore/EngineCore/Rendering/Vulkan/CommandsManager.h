#pragma once

#include <GLFW/glfw3.h>
#include <thread>
#include <mutex>

#include "VulkanContext.h"
#include "EngineCore/Core/Ref.h"

namespace AVulkan
{
    class CommandsManager
    {
    public:
        CommandsManager(const Ref<VulkanContext>& context, const int frameInFight);
        ~CommandsManager();

        void CreateCommandPool();
        VkCommandPool& GetCommandPool();
        VkCommandBuffer& GetCommandBuffer(int frame);
        
    private:
        Ref<VulkanContext> context;
        const int frameInFight;
        
        std::unordered_map<std::thread::id, uint32_t> threadsMapping;
        std::vector<VkCommandPool> commandPools;
        std::vector<std::vector<VkCommandBuffer>> commandBuffers;
        
        URef<std::mutex> createPoolMutex;
    };
}
