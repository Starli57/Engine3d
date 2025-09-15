#include "EngineCore/Pch.h"
#include "RenderPassUtility.h"

#include "EngineCore/CustomAssert.h"
#include "spdlog/spdlog.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FormatUtility.h"

namespace VulkanApi
{
	VkRenderPass CreateRenderPass(Ref<VulkanContext> vulkanContext,
		const std::vector<VkAttachmentDescription>& attachments, const VkSubpassDescription& subpass)
	{
		std::array<VkSubpassDependency, 2> subpassDependencies;
		subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		subpassDependencies[0].dstSubpass = 0;
		subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpassDependencies[0].dependencyFlags = 0;

		subpassDependencies[1].srcSubpass = 0;
		subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		subpassDependencies[1].dependencyFlags = 0;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
		renderPassInfo.pDependencies = subpassDependencies.data();

		VkRenderPass renderPass;
		auto createStatus = vkCreateRenderPass(vulkanContext->logicalDevice, &renderPassInfo, nullptr, &renderPass);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create render pass, status: " + createStatus);

		return renderPass;
	}

	void DisposeRenderPass(const VkDevice& logicalDevice, const VkRenderPass& renderPass)
	{
		spdlog::info("Dispose render pass");
		vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
	}

	void BeginRenderPass(const std::vector<VkClearValue>& clearValues, const VkFramebuffer& frameBuffer, const VkRenderPass& renderPass,
		const VkCommandBuffer& commandBuffer, const VkExtent2D& vkExtent)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = frameBuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vkExtent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void EndRenderPass(const VkCommandBuffer& commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer);
	}
}