#pragma once
#include "Core/RenderPassContext.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
using namespace EngineCore;

namespace AVulkan
{
    class PreRenderPass
    {
    public:
        PreRenderPass(const Ref<RenderPassContext>& renderPassContext);
        void UpdateDrawingEntities() const;
    private:
        Ref<RenderPassContext> renderPassContext;
        void Sort(std::vector<DrawEntity>& entities, const glm::vec3& rendererPosition) const;
    };
}
