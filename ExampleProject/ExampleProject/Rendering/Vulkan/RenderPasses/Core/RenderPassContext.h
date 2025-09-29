#pragma once
#include <vector>
#include "DrawEntity.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapchainContext.h"

namespace ClientVulkanApi
{
    class RenderPassContext
    {
    public:
        VulkanApi::DescriptorsManager* descriptorsManager;
        Ref<Ecs> ecs;
        Ref<Engine::ResourcesStorageVulkan> assetsDatabase;
        VulkanApi::SwapchainContext* swapchainContext;
        
        std::vector<DrawEntity> transparentEntities;
        std::vector<DrawEntity> opaqueEntities;

        RenderPassContext(VulkanApi::DescriptorsManager* descriptorsManager, const Ref<Ecs>& ecs,
                          const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase, VulkanApi::SwapchainContext* swapchainContext)
            : descriptorsManager(descriptorsManager), ecs(ecs), assetsDatabase(assetsDatabase), swapchainContext(swapchainContext) {}
    };
}
