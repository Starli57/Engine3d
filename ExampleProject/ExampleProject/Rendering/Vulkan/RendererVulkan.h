#pragma once
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "RenderPasses/PreRenderPass.h"
#include "RenderPasses/RenderPassClean.h"
#include "RenderPasses/RenderPassOpaque.h"
#include "RenderPasses/RenderPassShadowMaps.h"

namespace AVulkan
{
    class RendererVulkan
    {
    public:
        RendererVulkan(GraphicsApiVulkan* graphicsApiVulkan, const Ref<ResourcesStorageVulkan>& assetsDatabase, const Ref<Ecs>& ecs);
    
    private:
        GraphicsApiVulkan* graphicsApi;
        Ref<ResourcesStorageVulkan> assetsDatabase;
        Ref<Ecs> ecs;

        Ref<RenderPassContext> renderPassContext;
        Ref<PreRenderPass> preRenderPass;
        RenderPassClean* renderPassClean;
        RenderPassOpaque* renderPassOpaque;
        RenderPassShadowMaps* renderPassShadowMaps;
    
        void Render() const;
    
        void CreateRenderPasses();
        void DisposeRenderPasses() const;
    };
}