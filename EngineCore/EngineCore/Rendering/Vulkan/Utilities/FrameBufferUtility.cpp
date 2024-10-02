#include "EngineCore/Pch.h"
#include "FrameBufferUtility.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
    void CreateFrameBuffer(VkDevice& logicalDevice, VkRenderPass& renderPass, AVulkan::SwapChainData& swapChainData, 
        Ref<AVulkan::ImageModel> msaaColorBuffer, Ref<AVulkan::ImageModel> msaaDepthBuffer)
    {
        spdlog::info("Create frame buffers");
        swapChainData.frameBuffers.resize(swapChainData.imagesCount);

        for (size_t i = 0; i < swapChainData.imagesCount; i++)
        {
            std::array<VkImageView, 3> attachments =
            {
                msaaColorBuffer->imageView,
                msaaDepthBuffer->imageView,
                swapChainData.imageViews[i]
                
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainData.extent.width;
            framebufferInfo.height = swapChainData.extent.height;
            framebufferInfo.layers = 1;

            auto createStatus = vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainData.frameBuffers[i]);
            CAssert::Check(createStatus == VK_SUCCESS, "Failed to create framebuffer, status = " + createStatus);
        }
    }

    void DisposeFrameBuffer(VkDevice& logicalDevice, std::vector<VkFramebuffer>& frameBuffers)
    {
        spdlog::info("Dispose frame buffers");
        for (auto framebuffer : frameBuffers)
        {
            vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
        }
        frameBuffers.clear();
    }
}