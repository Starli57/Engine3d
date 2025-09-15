#pragma once
#include "Core/RenderPassContext.h"
#include "EngineCore/Core/Ref.h"

namespace VulkanApi
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
