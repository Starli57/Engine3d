#include "RendererVulkan.h"

#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkCommandBufferWrapper.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkImageWrapper.h"

namespace ClientVulkanApi
{
    RendererVulkan::RendererVulkan(Engine::EngineContext* engineContext)
        : engineContext(engineContext)
    {
        engineContext->renderer->BindClientFunctions(
            [this]() { Render(); },
            [this]() { CreateRenderPasses(); },
            [this]() { DisposeRenderPasses(); },
            [this]() { CreateDescriptors(); },
            [this]() { DisposeDescriptors(); }
            );
    
        renderPassClean = nullptr;
        renderPassOpaque = nullptr;
        renderPassShadowMaps = nullptr;
    }

    void RendererVulkan::Render() const
    {
        auto swapChainData = engineContext->renderer->vulkanContext->swapchainContext;
        const auto frame = engineContext->renderer->GetFrame();
        const auto imageIndex = engineContext->renderer->GetImageIndex();
        auto commandBuffer = engineContext->renderer->GetCommandBuffer();
    
        descriptorsManager->UpdateFrameDescriptors(frame);
        descriptorsManager->UpdateMaterialsDescriptors(frame);
        preRenderPass->UpdateDrawingEntities();

        VulkanApi::BeginCommandBuffer(commandBuffer);
        renderPassClean->Render(commandBuffer, frame, imageIndex, nullptr);
        renderPassShadowMaps->Render(commandBuffer, frame, imageIndex, nullptr);

        VulkanApi::TransitionImageLayout(commandBuffer, 1, renderPassShadowMaps->GetImageBuffer()->image,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

        descriptorsManager->UpdateShadowsMapDescriptors(frame, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
        renderPassOpaque->Render(commandBuffer, frame, imageIndex, [this](const Ref<Entity>& entity) { return true; });

        VulkanApi::TransitionImageLayout(commandBuffer, 1, swapChainData->images.at(imageIndex),
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT);

        VulkanApi::EndCommandBuffer(commandBuffer);
    }

    void RendererVulkan::CreateRenderPasses()
    {
        auto vulkanContext = engineContext->renderer->vulkanContext;
        auto swapChainData = vulkanContext->swapchainContext;

        renderPassContext = CreateRef<RenderPassContext>(descriptorsManager, engineContext->ecs, engineContext->resourcesStorage, swapChainData);
        
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

    void RendererVulkan::CreateDescriptors()
    {
        descriptorsManager = new DescriptorsManager(engineContext);
    }

    void RendererVulkan::DisposeDescriptors()
    {
        delete descriptorsManager;
    }
}
