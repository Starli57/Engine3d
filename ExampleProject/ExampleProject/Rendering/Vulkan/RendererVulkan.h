#pragma once
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "RenderPasses/PreRenderPass.h"
#include "RenderPasses/RenderPassClean.h"
#include "RenderPasses/RenderPassOpaque.h"
#include "RenderPasses/RenderPassShadowMaps.h"

namespace ClientVulkanApi
{
    class RendererVulkan
    {
    public:
        RendererVulkan(Engine::GraphicsApiVulkan* graphicsApiVulkan, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase, const Ref<Ecs>& ecs);
    
    private:
        Engine::GraphicsApiVulkan* graphicsApi;
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