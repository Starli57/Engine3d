#include "EngineCore/Pch.h"
#include "FrameBufferUtility.h"
#include "spdlog/spdlog.h"

namespace VkUtils
{
    void CreateFrameBuffer(VkDevice& logicalDevice, VkRenderPass& renderPass,
        VkExtent2D& extent, std::vector<VkImageView> attachments, VkFramebuffer& frameBuffer)
    {
        spdlog::info("Create frame buffers");

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        auto createStatus = vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &frameBuffer);
        CAssert::Check(createStatus == VK_SUCCESS, "Failed to create framebuffer, status = " + createStatus);
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

    void DisposeFrameBuffer(VkDevice& logicalDevice, VkFramebuffer& frameBuffer)
    {
        vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
    }
}