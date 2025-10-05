#pragma once
#include "Descriptors/DescriptorsManager.h"
#include "EngineCore/EngineContext.h"
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
        RendererVulkan(Engine::EngineContext* engineContext);
    
    private:
        Engine::EngineContext* engineContext;
        
        Ref<RenderPassContext> renderPassContext;
        Ref<PreRenderPass> preRenderPass;
        RenderPassClean* renderPassClean;
        RenderPassOpaque* renderPassOpaque;
        RenderPassShadowMaps* renderPassShadowMaps;

        DescriptorsManager* descriptorsManager;
        
        void Render() const;
    
        void CreateRenderPasses();
        void DisposeRenderPasses() const;

        void CreateDescriptors();
        void DisposeDescriptors();
    };
}