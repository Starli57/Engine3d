#pragma once
#include <vector>
#include "DrawEntity.h"
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"

namespace AVulkan
{
    class RenderPassContext
    {
    public:
        Ref<DescriptorsManager> descriptorsManager;
        Ref<Ecs> ecs;
        Ref<AssetsDatabaseVulkan> assetsDatabase;
        Ref<SwapChainData> swapChainData;
        
        std::vector<DrawEntity> transparentEntities;
        std::vector<DrawEntity> opaqueEntities;

        RenderPassContext(const Ref<DescriptorsManager>& descriptorsManager, const Ref<Ecs>& ecs,
            const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData)
            : descriptorsManager(descriptorsManager), ecs(ecs), assetsDatabase(assetsDatabase), swapChainData(swapChainData) {}
    };
}
