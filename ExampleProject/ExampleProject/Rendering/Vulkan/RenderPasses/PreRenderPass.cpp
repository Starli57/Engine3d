#include "EngineCore/Pch.h"
#include "PreRenderPass.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"

namespace AVulkan
{
    PreRenderPass::PreRenderPass(const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase)
        : ecs(ecs), assetsDatabase(assetsDatabase)
    {
    }

    void PreRenderPass::UpdateDrawingEntities() const
    {
        glm::vec3 rendererPosition;
        auto cameraEntries = ecs->registry->view<PositionComponent, CameraComponent, UboWorldComponent>();
        if (cameraEntries.begin() != cameraEntries.end())
        {
            auto firstCamera = cameraEntries.front();
            rendererPosition = cameraEntries.get<PositionComponent>(firstCamera).position;
        }
        
        std::ranges::sort(ecs->allEntities, [rendererPosition](const Ref<Entity>& a, const Ref<Entity>& b)
        {
            PositionComponent positionComponentA;
            PositionComponent positionComponentB;
            const auto gotA = a->TryGetComponent(positionComponentA);
            const auto gotB = b->TryGetComponent(positionComponentB);
            if (gotA == false || gotB == false) return false;
            
            return distance(positionComponentA.position, rendererPosition) > distance(positionComponentB.position, rendererPosition);
        });
    }
}
