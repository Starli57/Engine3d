#include "EngineCore/Pch.h"
#include "PreRenderPass.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Profiler/Profiler.h"

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

        Profiler::GetInstance().BeginSample("Prepass: Sort");
        std::ranges::sort(ecs->allEntities, [this, rendererPosition](const Ref<Entity>& a, const Ref<Entity>& b)
        {
            PositionComponent positionComponentA;
            PositionComponent positionComponentB;
            if (a->TryGetComponent(positionComponentA) == false ||  b->TryGetComponent(positionComponentB) == false) return false;

            MeshComponent meshComponentA;
            MeshComponent meshComponentB;
            if (a->TryGetComponent(meshComponentA) == false ||  b->TryGetComponent(meshComponentB) == false) return false;
            if (meshComponentA.meshIndex.has_value() == false || meshComponentB.meshIndex.has_value() == false) return false;
            
            return distance(positionComponentA.position + assetsDatabase->boundingBoxCenter.at(meshComponentA.meshIndex.value()), rendererPosition)
                > distance(positionComponentB.position + assetsDatabase->boundingBoxCenter.at(meshComponentB.meshIndex.value()), rendererPosition);
        });
        Profiler::GetInstance().EndSample();
    }
}
