#pragma once
#include <vector>
#include "DrawEntity.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"

namespace AVulkan
{
    class RenderPassContext
    {
    public:
        DescriptorsManager* descriptorsManager;
        Ref<Ecs> ecs;
        Ref<ResourcesStorageVulkan> assetsDatabase;
        Ref<SwapChainData> swapChainData;
        
        std::vector<DrawEntity> transparentEntities;
        std::vector<DrawEntity> opaqueEntities;

        RenderPassContext(DescriptorsManager* descriptorsManager, const Ref<Ecs>& ecs,
            const Ref<ResourcesStorageVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData)
            : descriptorsManager(descriptorsManager), ecs(ecs), assetsDatabase(assetsDatabase), swapChainData(swapChainData) {}
    };
}
