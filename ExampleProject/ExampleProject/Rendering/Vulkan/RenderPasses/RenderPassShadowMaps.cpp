#include "EngineCore/Pch.h"
#include "RenderPassShadowMaps.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "Utility/DrawEntity.h"

namespace AVulkan
{
    RenderPassShadowMaps::RenderPassShadowMaps(
        Ref<VulkanContext> vulkanContext, Ref<DescriptorsManager> descriptorsManager,
        const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData) :
        IRenderPass(vulkanContext, descriptorsManager, ecs, assetsDatabase, swapChainData)
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

        VkUtils::DisposeFrameBuffer(vulkanContext->logicalDevice, frameBuffers);
        frameBuffers.clear();

        VkUtils::DisposeImageModel(vulkanContext->logicalDevice, shadowMapBufferModel);

        for(const auto& pipelinePair : pipelines)
            GraphicsPipelineUtility().Dispose(pipelinePair.second, vulkanContext->logicalDevice);
        pipelines.clear();

        VkUtils::DisposeRenderPass(vulkanContext->logicalDevice, renderPass);
    }

    void RenderPassShadowMaps::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter)
    {
		Profiler::GetInstance().BeginSample("RenderPassShadowMaps");
        BeginRenderPass(commandBuffer, 0, 0, 1);
        
        auto pipeline = pipelines.at("shadowPass");
        VkUtils::BindPipeline(commandBuffer, pipeline);
        
        auto descriptorSets = std::vector<VkDescriptorSet>();
        descriptorSets.resize(1);
        descriptorSets.at(0) = descriptorsManager->GetDescriptorSetFrame(frame);
        
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0,
            static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
        
        for (const auto& drawEntity : ecs->allEntities)
        {
            MeshComponent meshComponent;
            UboModelComponent uboModelComponent;
            if (!drawEntity->TryGetComponent(meshComponent)) continue;
            if (!drawEntity->TryGetComponent(uboModelComponent)) continue;
            if (meshComponent.meshIndex.has_value() == false) continue;
            
            const int32_t meshIndex = meshComponent.meshIndex.value();
            VkUtils::BindVertexAndIndexBuffers(commandBuffer, meshIndex, assetsDatabase);

            vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &uboModelComponent.model);
            
            vkCmdDrawIndexed(commandBuffer, assetsDatabase->indexesCount.at(meshIndex), 1, 0, 0, 0);
        }

        VkUtils::EndRenderPass(commandBuffer);
		Profiler::GetInstance().EndSample();
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

        renderPass = VkUtils::CreateRenderPass(vulkanContext, attachments, subpass);
    }

    void RenderPassShadowMaps::CreatePipelines()
    {
        const auto shadowPassPipelineConfig = vulkanContext->pipelinesCollection->pipelinesConfigs.at("shadowPass");

        auto descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();
        descriptorSetLayouts.resize(1);
        descriptorSetLayouts.at(0) = descriptorsManager->GetDescriptorSetLayoutFrame();
            
        auto shadowPassPipeline = GraphicsPipelineUtility().Create(
            shadowPassPipelineConfig, vulkanContext->logicalDevice, renderPass,
            swapChainData->extent, descriptorSetLayouts, VK_SAMPLE_COUNT_1_BIT);
        pipelines.emplace(shadowPassPipelineConfig->pipelineName, shadowPassPipeline);
    }

    void RenderPassShadowMaps::CreateShadowMapBuffer()
    {
        spdlog::info("Create shadow map buffer");

        shadowMapBufferModel = CreateRef<ImageModel>();

        shadowMapBufferModel->image = VkUtils::CreateImage(
            vulkanContext, swapChainData->extent.width, swapChainData->extent.height, vulkanContext->depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_SAMPLE_COUNT_1_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            shadowMapBufferModel->imageMemory);

        VkUtils::CreateImageView(vulkanContext->logicalDevice, vulkanContext->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
            shadowMapBufferModel->image, shadowMapBufferModel->imageView);
    }

    void RenderPassShadowMaps::CreateFrameBuffers()
    {
        const std::vector<VkImageView> attachments =
        {
            shadowMapBufferModel->imageView
        };

        frameBuffers.resize(1);
        VkUtils::CreateFrameBuffer(vulkanContext->logicalDevice, renderPass,
            swapChainData->extent.width, swapChainData->extent.height, attachments, frameBuffers[0]);
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
        CAssert::Check(createStatus == VK_SUCCESS, "Shadow map sampler can't be created");
    }

}
