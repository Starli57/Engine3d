#pragma once
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "RenderPasses/PreRenderPass.h"
#include "RenderPasses/RenderPassClean.h"
#include "RenderPasses/RenderPassOpaque.h"
#include "RenderPasses/RenderPassShadowMaps.h"

class RendererVulkan
{
public:
    RendererVulkan(AVulkan::GraphicsApiVulkan* graphicsApiVulkan, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<Ecs>& ecs);
    
private:
    AVulkan::GraphicsApiVulkan* graphicsApi;
    Ref<AssetsDatabaseVulkan> assetsDatabase;
    Ref<Ecs> ecs;
    
    Ref<AVulkan::PreRenderPass> preRenderPass;
    AVulkan::RenderPassClean* renderPassClean;
    AVulkan::RenderPassOpaque* renderPassOpaque;
    AVulkan::RenderPassShadowMaps* renderPassShadowMaps;
    
    void Render() const;
    
    void CreateRenderPasses();
    void DisposeRenderPasses() const;
};
