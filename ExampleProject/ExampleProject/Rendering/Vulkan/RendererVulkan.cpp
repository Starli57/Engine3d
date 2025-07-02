#include "RendererVulkan.h"

#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"

RendererVulkan::RendererVulkan(AVulkan::GraphicsApiVulkan* graphicsApiVulkan, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<Ecs>& ecs)
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
    auto descriptorsManager = graphicsApi->descriptorsManager;
    auto swapChainData = graphicsApi->swapChainData;
    auto vulkanContext = graphicsApi->context;
    const auto frame = graphicsApi->GetFrame();
    const auto imageIndex = graphicsApi->GetImageIndex();
    auto commandBuffer = graphicsApi->GetCommandBuffer();
    
    descriptorsManager->UpdateFrameDescriptors(frame);
    descriptorsManager->UpdateMaterialsDescriptors(frame);
    preRenderPass->UpdateDrawingEntities();
		
    VkUtils::BeginCommandBuffer(commandBuffer);
    renderPassClean->Render(commandBuffer, frame, imageIndex, nullptr);
    renderPassShadowMaps->Render(commandBuffer, frame, imageIndex, nullptr);

    VkUtils::TransitionImageLayout(commandBuffer, renderPassShadowMaps->GetImageBuffer()->image,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

    descriptorsManager->UpdateShadowsMapDescriptors(frame, renderPassShadowMaps->GetImageBuffer()->imageView, renderPassShadowMaps->GetSampler());
    renderPassOpaque->Render(commandBuffer, frame, imageIndex, [this](const Ref<Entity>& entity) { return true; });
		
    VkUtils::TransitionImageLayout(commandBuffer, swapChainData->images.at(imageIndex),
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT);

    VkUtils::EndCommandBuffer(commandBuffer);
}

void RendererVulkan::CreateRenderPasses()
{
    auto descriptorsManager = graphicsApi->descriptorsManager;
    auto swapChainData = graphicsApi->swapChainData;
    auto vulkanContext = graphicsApi->context;
    
    preRenderPass = CreateRef<AVulkan::PreRenderPass>(ecs, assetsDatabase);
    renderPassClean = new AVulkan::RenderPassClean(vulkanContext, descriptorsManager, ecs, assetsDatabase, swapChainData);
    renderPassShadowMaps = new AVulkan::RenderPassShadowMaps(vulkanContext, descriptorsManager, ecs, assetsDatabase, swapChainData);
    renderPassOpaque = new AVulkan::RenderPassOpaque(vulkanContext, descriptorsManager, ecs, assetsDatabase, swapChainData);
}

void RendererVulkan::DisposeRenderPasses() const
{
    delete renderPassClean;
    delete renderPassOpaque;
    delete renderPassShadowMaps;
}
