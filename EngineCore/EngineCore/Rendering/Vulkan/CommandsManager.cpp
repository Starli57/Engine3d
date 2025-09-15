#include "EngineCore/Pch.h"
#include "CommandsManager.h"

#include "Utilities/CommandBufferUtility.h"
#include "Utilities/CommandPoolUtility.h"


namespace AVulkan
{
    CommandsManager::CommandsManager(const Ref<VulkanContext>& context, const int frameInFight)
        : context(context), frameInFight(frameInFight)
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
            VulkanApi::FreeCommandBuffers(context->logicalDevice, commandPools.at(i), commandBuffers.at(i));
            VulkanApi::DisposeCommandPool(context, commandPools[i]);
        }
        
        commandBuffers.clear();
        commandPools.clear();
    }

    void CommandsManager::CreateCommandPool()
    {
        createPoolMutex->lock();
        auto threadId = std::this_thread::get_id();
        auto poolIndex = commandPools.size();
        threadsMapping.emplace(threadId, poolIndex);
        
        commandPools.push_back(VulkanApi::CreateCommandPool(context));
        commandBuffers.emplace_back(frameInFight);
        VulkanApi::AllocateCommandBuffers(context->logicalDevice, commandPools.at(poolIndex), commandBuffers.at(poolIndex), frameInFight);
        createPoolMutex->unlock();
    }

    VkCommandPool& CommandsManager::GetCommandPool()
    {
        const auto threadId = std::this_thread::get_id();
        if (!threadsMapping.contains(threadId)) CreateCommandPool();
        
        return commandPools.at(threadsMapping.at(threadId));
    }

    VkCommandBuffer& CommandsManager::GetCommandBuffer(const int frame)
    {
        const auto threadId = std::this_thread::get_id();
        GetCommandPool();
        return commandBuffers.at(threadsMapping.at(threadId)).at(frame);
    }
}
