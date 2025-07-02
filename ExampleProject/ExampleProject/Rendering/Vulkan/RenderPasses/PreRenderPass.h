#pragma once
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "Utility/DrawEntity.h"
using namespace EngineCore;

namespace AVulkan
{
    class PreRenderPass
    {
    public:
        PreRenderPass(const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase);
        void UpdateDrawingEntities() const;
    private:
        Ref<Ecs> ecs;
        Ref<AssetsDatabaseVulkan> assetsDatabase;
    };
}
