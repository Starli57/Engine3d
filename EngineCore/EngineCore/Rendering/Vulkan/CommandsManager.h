#pragma once

#include <GLFW/glfw3.h>
#include <thread>
#include <mutex>

#include "EngineCore/Core/Ref.h"

namespace AVulkan
{
    class CommandsManager
    {
    public:
        CommandsManager(const VkPhysicalDevice& physicalDevice, const VkDevice& logicalDevice, const VkSurfaceKHR& windowSurface, const int frameInFight);
        ~CommandsManager();

        void CreateCommandPool();
        VkCommandPool& GetCommandPool();
        VkCommandBuffer& GetCommandBuffer(int frame);
        
    private:
        const VkPhysicalDevice& physicalDevice;
        const VkDevice& logicalDevice;
        const VkSurfaceKHR& windowSurface;
        const int frameInFight;
        
        std::unordered_map<std::thread::id, uint32_t> threadsMapping;
        std::vector<VkCommandPool> commandPools;
        std::vector<std::vector<VkCommandBuffer>> commandBuffers;
        
        URef<std::mutex> createPoolMutex;
    };
}
