#include "EngineCore/Pch.h"
#include "CommandsManager.h"

#include "ApiWrappers/VkCommandBufferWrapper.h"
#include "ApiWrappers/VkCommandPoolWrapper.h"


namespace VulkanApi
{
    CommandsManager::CommandsManager(VulkanContext* vulkanContext, const int buffersCount)
        : vulkanContext(vulkanContext), buffersCount(buffersCount)
    {
        const auto coresCount = std::thread::hardware_concurrency();
        commandPools.reserve(coresCount);
        commandBuffers.reserve(coresCount);
        
        createPoolMutex = CreateUniqueRef<std::mutex>();
    }

    CommandsManager::~CommandsManager()
    {
        for (int i = 0; i < commandPools.size(); i++)
        {
            FreeCommandBuffers(vulkanContext->logicalDevice, commandPools.at(i), commandBuffers.at(i));
            DisposeCommandPool(vulkanContext, commandPools[i]);
        }
        
        commandBuffers.clear();
        commandPools.clear();
    }

    void CommandsManager::CreateCommandPool(VkCommandPool& outCommandPool)
    {
        createPoolMutex->lock();
        auto threadId = std::this_thread::get_id();
        auto poolIndex = commandPools.size();
        threadsMapping.emplace(threadId, poolIndex);

        VulkanApi::CreateCommandPool(vulkanContext, outCommandPool);
        commandPools.emplace_back(outCommandPool);
        
        commandBuffers.emplace_back(buffersCount);
        AllocateCommandBuffers(vulkanContext->logicalDevice, commandPools.at(poolIndex), commandBuffers.at(poolIndex), buffersCount);
        createPoolMutex->unlock();
    }

    void CommandsManager::RefCommandPool(VkCommandPool& outCommandPool)
    {
        const auto threadId = std::this_thread::get_id();
        if (threadsMapping.contains(threadId)) outCommandPool = commandPools.at(threadsMapping.at(threadId));
        else CreateCommandPool(outCommandPool);
    }

    void CommandsManager::RefCommandBuffer(const int frame, VkCommandBuffer& outCommandBuffer)
    {
        const auto threadId = std::this_thread::get_id();
        VkCommandPool commandPool;
        RefCommandPool(commandPool);
        outCommandBuffer = commandBuffers.at(threadsMapping.at(threadId)).at(frame);
    }
}
