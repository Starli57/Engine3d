#pragma once
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#include "RenderPasses/PreRenderPass.h"
#include "RenderPasses/RenderPassClean.h"
#include "RenderPasses/RenderPassOpaque.h"
#include "RenderPasses/RenderPassShadowMaps.h"

namespace ClientVulkanApi
{
    class RendererVulkan
    {
    public:
        RendererVulkan(Engine::RendererVulkan* renderingEngine, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase, const Ref<Ecs>& ecs);
    
    private:
        Engine::RendererVulkan* renderingEngine;
        Ref<Engine::ResourcesStorageVulkan> assetsDatabase;
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