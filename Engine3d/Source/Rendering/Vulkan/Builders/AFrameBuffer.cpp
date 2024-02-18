#include "Pch.h"
#include "AFrameBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
    void AFrameBuffer::Create(VkDevice& logicalDevice, VkRenderPass& renderPass, SwapChainData& swapChainData) const
    {
        spdlog::info("Create frame buffers");
        swapChainData.frameBuffers.resize(swapChainData.imageViews.size());

        for (size_t i = 0; i < swapChainData.imageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                swapChainData.imageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainData.extent.width;
            framebufferInfo.height = swapChainData.extent.height;
            framebufferInfo.layers = 1;

            auto createStatus = vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainData.frameBuffers[i]);
            CAssert::Check(createStatus == VK_SUCCESS, "Failed to create framebuffer, status = " + createStatus);
        }
    }

    void AFrameBuffer::Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const
    {
        spdlog::info("Dispose frame buffers");
        for (auto framebuffer : swapChainData.frameBuffers)
        {
            vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
        }
    }
}