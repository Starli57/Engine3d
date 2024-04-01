#include "Pch.h"
#include "AFrameBuffer.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
    void AFrameBuffer::Create(Ref<VulkanModel> vulkanModel) const
    {
        spdlog::info("Create frame buffers");
        vulkanModel->swapChainData->frameBuffers.resize(vulkanModel->swapChainData->imageViews.size());

        for (size_t i = 0; i < vulkanModel->swapChainData->imageViews.size(); i++)
        {
            std::array<VkImageView, 2> attachments =
            {
                vulkanModel->swapChainData->imageViews[i],
                vulkanModel->swapChainData->depthBufferModel->imageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vulkanModel->renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = vulkanModel->swapChainData->extent.width;
            framebufferInfo.height = vulkanModel->swapChainData->extent.height;
            framebufferInfo.layers = 1;

            auto createStatus = vkCreateFramebuffer(vulkanModel->logicalDevice, &framebufferInfo, nullptr, &vulkanModel->swapChainData->frameBuffers[i]);
            CAssert::Check(createStatus == VK_SUCCESS, "Failed to create framebuffer, status = " + createStatus);
        }
    }

    void AFrameBuffer::Dispose(Ref<VulkanModel> vulkanModel) const
    {
        spdlog::info("Dispose frame buffers");
        for (auto framebuffer : vulkanModel->swapChainData->frameBuffers)
        {
            vkDestroyFramebuffer(vulkanModel->logicalDevice, framebuffer, nullptr);
        }
        vulkanModel->swapChainData->frameBuffers.clear();
    }
}