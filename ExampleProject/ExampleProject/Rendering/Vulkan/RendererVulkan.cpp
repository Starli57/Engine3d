#include "RendererVulkan.h"

#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"

namespace AVulkan
{
    RendererVulkan::RendererVulkan(AVulkan::GraphicsApiVulkan* graphicsApiVulkan, const Ref<ResourcesStorageVulkan>& assetsDatabase, const Ref<Ecs>& ecs)
        : graphicsApi(graphicsApiVulkan), assetsDatabase(assetsDatabase), ecs(ecs)
    {
        graphicsApi->BindClientFunctions(
            [this]() { Render(); },
            [this]() { CreateRenderPasses(); },
            [this]() { DisposeRenderPasses(); });
    
        renderPassClean = nullptr;
        renderPassOpaque = nullptr;
        renderPassShadowMaps = nullptr;
    }

    void RendererVulkan::Render() const
    {
        auto swapChainData = graphicsApi->swapChainData;
        auto vulkanContext = graphicsApi->context;
        const auto frame = graphicsApi->GetFrame();
        const auto imageIndex = graphicsApi->GetImageIndex();
        auto commandBuffer = graphicsApi->GetCommandBuffer();
    
        graphicsApi->descriptorsManager->UpdateFrameDescriptors(frame);
        graphicsApi->descriptorsManager->UpdateMaterialsDescriptors(frame);
        preRenderPass->UpdateDrawingEntities();
		
        VkUtils::BeginCommandBuffer(commandBuffer);
        renderPassClean->Render(commandBuffer, frame, imageIndex, nullptr);
        renderPassShadowMaps->Render(commandBuffer, frame, imageIndex, nullptr);

        VkUtils::TransitionImageLayout(commandBuffer, renderPassShadowMaps->GetImageBuffer()->image,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

        graphicsApi->descriptorsManager->UpdateShadowsMapDescriptors(frame, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
        renderPassOpaque->Render(commandBuffer, frame, imageIndex, [this](const Ref<Entity>& entity) { return true; });
		
        VkUtils::TransitionImageLayout(commandBuffer, swapChainData->images.at(imageIndex),
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT);

        VkUtils::EndCommandBuffer(commandBuffer);
    }

    void RendererVulkan::CreateRenderPasses()
    {
        auto swapChainData = graphicsApi->swapChainData;
        auto vulkanContext = graphicsApi->context;

        renderPassContext = CreateRef<RenderPassContext>(graphicsApi->descriptorsManager.get(), ecs, assetsDatabase, swapChainData);
        
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