#include "EngineCore/Pch.h"
#include "PreRenderPass.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Profiler/Profiler.h"

namespace ClientVulkanApi
{
    PreRenderPass::PreRenderPass(const Ref<RenderPassContext>& renderPassContext) : renderPassContext(renderPassContext)
    {
    }

    void PreRenderPass::UpdateDrawingEntities() const
    {
        glm::vec3 rendererPosition;
        auto cameraEntries = renderPassContext->ecs->registry->view<PositionComponent, CameraComponent, UboWorldComponent>();
        if (cameraEntries.begin() != cameraEntries.end())
        {
            auto firstCamera = cameraEntries.front();
            rendererPosition = cameraEntries.get<PositionComponent>(firstCamera).position;
        }

        uint16_t transparentCount = 0;
        uint16_t opaqueCount = 0;
        auto renderEntities = renderPassContext->ecs->registry->view<PositionComponent, MaterialComponent, MeshComponent, UboModelComponent>();
        for (const auto& entity : renderEntities)
        {
            MaterialComponent& materialComponent = renderEntities.get<MaterialComponent>(entity);
            auto material = renderPassContext->assetsDatabase->materials.at(materialComponent.materialIndex);

            if (material->opaque) opaqueCount++;
            else transparentCount++;
        }

        renderPassContext->opaqueEntities.resize(opaqueCount);
        renderPassContext->transparentEntities.resize(transparentCount);
        opaqueCount = 0;
        transparentCount = 0;
        for (const auto& entity : renderEntities)
        {
            PositionComponent& positionComponent = renderEntities.get<PositionComponent>(entity);
            MaterialComponent& materialComponent = renderEntities.get<MaterialComponent>(entity);
            MeshComponent& meshComponent = renderEntities.get<MeshComponent>(entity);
            UboModelComponent& uboModel = renderEntities.get<UboModelComponent>(entity);
            
            auto material = renderPassContext->assetsDatabase->materials.at(materialComponent.materialIndex);
            auto isOpaque = material->opaque;
            
            DrawEntity drawEntity = DrawEntity(&positionComponent, &uboModel, &meshComponent, &materialComponent);
            if (isOpaque) renderPassContext->opaqueEntities[opaqueCount++] = drawEntity;
            else renderPassContext->transparentEntities[transparentCount++] = drawEntity;

            Engine::Profiler::GetInstance().AddTrianglesCount(renderPassContext->assetsDatabase->indexesCount.at(meshComponent.meshIndex.value()));
        }

        Engine::Profiler::GetInstance().BeginSample("Prepass: Sort Opaque");
        Sort(renderPassContext->opaqueEntities, rendererPosition);
        Engine::Profiler::GetInstance().EndSample();

        Engine::Profiler::GetInstance().BeginSample("Prepass: Sort Transparent");
        Sort(renderPassContext->transparentEntities, rendererPosition);
        Engine::Profiler::GetInstance().EndSample();
    }

    void PreRenderPass::Sort(std::vector<DrawEntity>& entities, const glm::vec3& rendererPosition) const
    {
        std::ranges::sort(entities, [this, rendererPosition](const DrawEntity& a, const DrawEntity& b)
        {
            if (a.meshComponent->meshIndex.has_value() == false) return false;
            if (b.meshComponent->meshIndex.has_value() == false) return true;
            
            auto distanceA = distance(a.positionComponent->position + renderPassContext->assetsDatabase->boundingBoxCenter.at(a.meshComponent->meshIndex.value()), rendererPosition);
            auto distanceB = distance(b.positionComponent->position + renderPassContext->assetsDatabase->boundingBoxCenter.at(b.meshComponent->meshIndex.value()), rendererPosition);
            return distanceB > distanceA;
        });
    }
}
