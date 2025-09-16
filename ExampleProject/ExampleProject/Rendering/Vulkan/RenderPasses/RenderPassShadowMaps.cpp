#include "EngineCore/Pch.h"
#include "RenderPassShadowMaps.h"

#include "EngineCore/CustomAssert.h"
#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"

namespace ClientVulkanApi
{
    RenderPassShadowMaps::RenderPassShadowMaps(VulkanApi::VulkanContext* vulkanContext, const Ref<RenderPassContext>& renderPassContext) :
        IRenderPass(vulkanContext, renderPassContext)
    {
        spdlog::info("Create RenderPass ShadowMaps");

        RenderPassShadowMaps::CreateRenderPass();
        RenderPassShadowMaps::CreatePipelines();
        CreateShadowMapBuffer();
        RenderPassShadowMaps::CreateFrameBuffers();
        CreateSampler();    
    }

    RenderPassShadowMaps::~RenderPassShadowMaps()
    {
        spdlog::info("RenderPassShadowMaps::~RenderPassShadowMaps"); 
        vkDestroySampler(vulkanContext->logicalDevice, sampler, nullptr);

        VulkanApi::DisposeFrameBuffer(vulkanContext->logicalDevice, frameBuffers);
        frameBuffers.clear();

        DisposeImageModel(vulkanContext->logicalDevice, shadowMapBufferModel);

        for(const auto& pipelinePair : pipelines)
            VulkanApi::GraphicsPipelineUtility().Dispose(pipelinePair.second, vulkanContext->logicalDevice);
        pipelines.clear();

        VulkanApi::DisposeRenderPass(vulkanContext->logicalDevice, renderPass);
    }

    void RenderPassShadowMaps::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter)
    {
        Engine::Profiler::GetInstance().BeginSample("RenderPassShadowMaps");
        BeginRenderPass(commandBuffer, 0, 0, 1);
        
        auto pipeline = pipelines.at("shadowPass");
        VulkanApi::BindPipeline(commandBuffer, pipeline);
        
        auto descriptorSets = std::vector<VkDescriptorSet>();
        descriptorSets.resize(1);
        descriptorSets.at(0) = renderPassContext->descriptorsManager->GetDescriptorSetFrame(frame);
        
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0,
            static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
        
        for (const auto& drawEntity : renderPassContext->opaqueEntities)
        {
            RenderEntity(drawEntity, commandBuffer, pipeline);
        }

        for (const auto& drawEntity : renderPassContext->transparentEntities)
        {
            RenderEntity(drawEntity, commandBuffer, pipeline);
        }

        VulkanApi::EndRenderPass(commandBuffer);
        Engine::Profiler::GetInstance().EndSample();
    }

    void RenderPassShadowMaps::RenderEntity(const DrawEntity& drawEntity, const VkCommandBuffer& commandBuffer, const Ref<PipelineVulkan>& pipeline) const
    {
        if (drawEntity.meshComponent->meshIndex.has_value() == false) return;
        
        const int32_t meshIndex = drawEntity.meshComponent->meshIndex.value();
        VulkanApi::BindVertexAndIndexBuffers(commandBuffer, meshIndex, renderPassContext->assetsDatabase);

        vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &drawEntity.uboModelComponent->model);
            
        vkCmdDrawIndexed(commandBuffer, renderPassContext->assetsDatabase->indexesCount.at(meshIndex), 1, 0, 0, 0);
        Engine::Profiler::GetInstance().AddDrawCall();
    }

    void RenderPassShadowMaps::CreateRenderPass()
    {
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = vulkanContext->depthFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 0;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::vector<VkAttachmentDescription> attachments = { depthAttachment };

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 0;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        renderPass = VulkanApi::CreateRenderPass(vulkanContext, attachments, subpass);
    }

    void RenderPassShadowMaps::CreatePipelines()
    {
        const auto shadowPassPipelineConfig = vulkanContext->pipelinesCollection->pipelinesConfigs.at("shadowPass");

        auto descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();
        descriptorSetLayouts.resize(1);
        descriptorSetLayouts.at(0) = renderPassContext->descriptorsManager->GetDescriptorSetLayoutFrame();

        auto pipeline = CreateRef<PipelineVulkan>();
        VulkanApi::GraphicsPipelineUtility().Create(
            shadowPassPipelineConfig, vulkanContext->logicalDevice, renderPass,
            renderPassContext->swapChainData->extent, descriptorSetLayouts, VK_SAMPLE_COUNT_1_BIT, pipeline);
        pipelines.emplace(shadowPassPipelineConfig->pipelineName, pipeline);
    }

    void RenderPassShadowMaps::CreateShadowMapBuffer()
    {
        spdlog::info("Create shadow map buffer");

        shadowMapBufferModel = CreateRef<VulkanApi::ImageModel>();

        CreateImage(
            vulkanContext, renderPassContext->swapChainData->extent.width, renderPassContext->swapChainData->extent.height, 1, vulkanContext->depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_SAMPLE_COUNT_1_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            shadowMapBufferModel->image,
            shadowMapBufferModel->imageMemory);

        VulkanApi::CreateImageView(vulkanContext->logicalDevice, 1, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
            shadowMapBufferModel->image, shadowMapBufferModel->imageView);
    }

    void RenderPassShadowMaps::CreateFrameBuffers()
    {
        const std::vector<VkImageView> attachments =
        {
            shadowMapBufferModel->imageView
        };

        frameBuffers.resize(1);
        VulkanApi::CreateFrameBuffer(vulkanContext->logicalDevice, renderPass,
            renderPassContext->swapChainData->extent.width, renderPassContext->swapChainData->extent.height, attachments, frameBuffers[0]);
    }

    void RenderPassShadowMaps::CreateSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_TRUE;
        samplerInfo.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        auto createStatus = vkCreateSampler(vulkanContext->logicalDevice, &samplerInfo, nullptr, &sampler);
        Engine::CAssert::Check(createStatus == VK_SUCCESS, "Shadow map sampler can't be created");
    }

}
