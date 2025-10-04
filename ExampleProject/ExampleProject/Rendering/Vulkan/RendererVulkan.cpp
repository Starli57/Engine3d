#include "RendererVulkan.h"

#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkCommandBufferWrapper.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkImageWrapper.h"

namespace ClientVulkanApi
{
    RendererVulkan::RendererVulkan(Engine::RendererVulkan* renderingEngine, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase, const Ref<Ecs>& ecs)
        : renderingEngine(renderingEngine), assetsDatabase(assetsDatabase), ecs(ecs)
    {
        renderingEngine->BindClientFunctions(
            [this]() { Render(); },
            [this]() { CreateRenderPasses(); },
            [this]() { DisposeRenderPasses(); });
    
        renderPassClean = nullptr;
        renderPassOpaque = nullptr;
        renderPassShadowMaps = nullptr;
    }

    void RendererVulkan::Render() const
    {
        auto swapChainData = renderingEngine->vulkanContext->swapchainContext;
        const auto frame = renderingEngine->GetFrame();
        const auto imageIndex = renderingEngine->GetImageIndex();
        auto commandBuffer = renderingEngine->GetCommandBuffer();
    
        renderingEngine->descriptorsManager->UpdateFrameDescriptors(frame);
        renderingEngine->descriptorsManager->UpdateMaterialsDescriptors(frame);
        preRenderPass->UpdateDrawingEntities();

        VulkanApi::BeginCommandBuffer(commandBuffer);
        renderPassClean->Render(commandBuffer, frame, imageIndex, nullptr);
        renderPassShadowMaps->Render(commandBuffer, frame, imageIndex, nullptr);

        VulkanApi::TransitionImageLayout(commandBuffer, 1, renderPassShadowMaps->GetImageBuffer()->image,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

        renderingEngine->descriptorsManager->UpdateShadowsMapDescriptors(frame, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
        renderPassOpaque->Render(commandBuffer, frame, imageIndex, [this](const Ref<Entity>& entity) { return true; });

        VulkanApi::TransitionImageLayout(commandBuffer, 1, swapChainData->images.at(imageIndex),
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT);

        VulkanApi::EndCommandBuffer(commandBuffer);
    }

    void RendererVulkan::CreateRenderPasses()
    {
        auto vulkanContext = renderingEngine->vulkanContext;
        auto swapChainData = vulkanContext->swapchainContext;

        renderPassContext = CreateRef<RenderPassContext>(renderingEngine->descriptorsManager.get(), ecs, assetsDatabase, swapChainData);
        
        preRenderPass = CreateRef<PreRenderPass>(renderPassContext);
        renderPassClean = new RenderPassClean(vulkanContext, renderPassContext);
        renderPassShadowMaps = new RenderPassShadowMaps(vulkanContext, renderPassContext);
        renderPassOpaque = new RenderPassOpaque(vulkanContext, renderPassContext);
    }

    void RendererVulkan::DisposeRenderPasses() const
    {
        delete renderPassClean;
        delete renderPassOpaque;
        delete renderPassShadowMaps;
    }
}