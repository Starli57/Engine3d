#include "EngineCore/Pch.h"
#include "RenderPassClean.h"

#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"

namespace VulkanApi
{
    RenderPassClean::RenderPassClean(VulkanContext* vulkanContext, const Ref<RenderPassContext>& renderPassContext)
        : IRenderPass(vulkanContext, renderPassContext)
    {
        RenderPassClean::CreateRenderPass();
        RenderPassClean::CreateFrameBuffers();
    }

    RenderPassClean::~RenderPassClean()
    {
        DisposeFrameBuffer(vulkanContext->logicalDevice, frameBuffers);
        frameBuffers.clear();
        
        DisposeRenderPass(vulkanContext->logicalDevice, renderPass);
    }

    void RenderPassClean::Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex,
        std::function<bool(const Ref<Entity>& entity)> filter)
    {
        BeginRenderPass(commandBuffer, imageIndex, 2, 1);
        EndRenderPass(commandBuffer);
    }

    void RenderPassClean::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = vulkanContext->imageFormat;
        colorAttachment.samples = vulkanContext->msaa;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = vulkanContext->imageFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 1;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        
        std::vector<VkAttachmentDescription> attachments = { colorAttachment, colorAttachmentResolve };

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        renderPass = VulkanApi::CreateRenderPass(vulkanContext, attachments, subpass);
    }

    void RenderPassClean::CreatePipelines()
    {
    }

    void RenderPassClean::CreateFrameBuffers()
    {
        frameBuffers.resize(renderPassContext->swapChainData->imagesCount);

        for (size_t i = 0; i < renderPassContext->swapChainData->imagesCount; i++)
        {
            std::vector<VkImageView> attachments =
            {
                renderPassContext->swapChainData->msaaColorSample->imageView,
                renderPassContext->swapChainData->imageViews[i]
            };

            CreateFrameBuffer(vulkanContext->logicalDevice, renderPass, renderPassContext->swapChainData->extent.width,
                renderPassContext->swapChainData->extent.height, attachments, frameBuffers[i]);
        }
    }
}
