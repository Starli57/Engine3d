#include "EngineCore/Pch.h"
#include "RenderPassShadowMaps.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "Utility/DrawEntity.h"

namespace AVulkan
{
    RenderPassShadowMaps::RenderPassShadowMaps(
        VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig, Ref<DescriptorsManager> descriptorsManager,
        const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData,
        const Ref<PipelinesCollection>& pipelinesCollection) :
        IRenderPass(physicalDevice, logicalDevice, rendererConfig, descriptorsManager, ecs, assetsDatabase, swapChainData),
        pipelinesCollection(pipelinesCollection)
    {
        spdlog::info("Create RenderPass ShadowMaps");

        RenderPassShadowMaps::CreateRenderPass(rendererConfig);
        RenderPassShadowMaps::CreatePipelines();
        CreateShadowMapBuffer();
        RenderPassShadowMaps::CreateFrameBuffers();
        CreateSampler();    
    }

    RenderPassShadowMaps::~RenderPassShadowMaps()
    {
        spdlog::info("RenderPassShadowMaps::~RenderPassShadowMaps"); 
        vkDestroySampler(logicalDevice, sampler, nullptr);

        VkUtils::DisposeFrameBuffer(logicalDevice, frameBuffers);
        frameBuffers.clear();

        VkUtils::DisposeImageModel(logicalDevice, shadowMapBufferModel);

        for(const auto& pipelinePair : pipelines)
            GraphicsPipelineUtility().Dispose(pipelinePair.second, logicalDevice);
        pipelines.clear();

        VkUtils::DisposeRenderPass(this->logicalDevice, renderPass);
    }

    void RenderPassShadowMaps::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex)
    {
		Profiler::GetInstance().BeginSample("RenderPassShadowMaps");
        BeginRenderPass(commandBuffer, imageIndex);

        //todo: replace to 1 place for all render passes and call only once for all
        const auto entities = ecs->registry->view<PositionComponent, UboModelComponent, MeshComponent, MaterialComponent>();
        std::vector<DrawEntity> drawEntities = std::vector<DrawEntity>();
        for(const auto entity : entities)
        {
            auto [positionComponent, uboModelComponent, meshContainer, materialComponent] =
                entities.get<PositionComponent, UboModelComponent, MeshComponent, MaterialComponent>(entity);

            if (meshContainer.meshIndex.has_value() == false || assetsDatabase->meshLoadStatuses.at(meshContainer.meshIndex.value()) != 2) continue;
            if (assetsDatabase->materialLoadStatuses.at(materialComponent.materialIndex) != 2) continue;

            drawEntities.emplace_back(&positionComponent, &uboModelComponent, &meshContainer, &materialComponent);
        }
        
        auto descriptorSets = std::vector<VkDescriptorSet>();
        descriptorSets.resize(1);
        descriptorSets.at(0) = descriptorsManager->GetDescriptorSetFrame(frame);
        
        int i = 0;
        for (const auto drawEntity : drawEntities)
        {
            const int index = i * VulkanConfiguration::maxFramesInFlight + frame;

            if (drawEntity.meshComponent->meshIndex.has_value() == false) continue;
            
            const int32_t meshIndex = drawEntity.meshComponent->meshIndex.value();
            auto pipeline = pipelines.at("shadowPass");

            VkUtils::BindPipeline(commandBuffer, pipeline);
            VkUtils::BindVertexAndIndexBuffers(commandBuffer, meshIndex, assetsDatabase);

            vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &drawEntity.uboModelComponent->model);
            
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0,
                static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, assetsDatabase->indexesCount.at(meshIndex), 1, 0, 0, 0);
            i++;
        }

        VkUtils::EndRenderPass(commandBuffer);
		Profiler::GetInstance().EndSample("RenderPassShadowMaps");
    }

    void RenderPassShadowMaps::BeginRenderPass(VkCommandBuffer& commandBuffer, const uint32_t imageIndex)
    {
        std::vector<VkClearValue> clearValues;
        clearValues.resize(1);

        clearValues[0].depthStencil.depth = 1.0f;

        VkUtils::BeginRenderPass(clearValues, frameBuffers[0], renderPass, commandBuffer, swapChainData->extent);
    }

    void RenderPassShadowMaps::CreateRenderPass(Ref<VulkanConfiguration> rendererConfig)
    {
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = rendererConfig->depthFormat;
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

        renderPass = VkUtils::CreateRenderPass(physicalDevice, logicalDevice, rendererConfig, attachments, subpass);
    }

    void RenderPassShadowMaps::CreatePipelines()
    {
        const auto shadowPassPipelineConfig = pipelinesCollection->pipelinesConfigs.at("shadowPass");

        auto descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();
        descriptorSetLayouts.resize(1);
        descriptorSetLayouts.at(0) = descriptorsManager->GetDescriptorSetLayoutFrame();
            
        auto shadowPassPipeline = GraphicsPipelineUtility().Create(
            shadowPassPipelineConfig, logicalDevice, renderPass,
            swapChainData->extent, descriptorSetLayouts, VK_SAMPLE_COUNT_1_BIT);
        pipelines.emplace(shadowPassPipelineConfig->pipelineName, shadowPassPipeline);
    }

    void RenderPassShadowMaps::CreateShadowMapBuffer()
    {
        spdlog::info("Create shadow map buffer");

        shadowMapBufferModel = CreateRef<ImageModel>();

        shadowMapBufferModel->image = VkUtils::CreateImage(
            physicalDevice, logicalDevice,
            swapChainData->extent.width, swapChainData->extent.height,
            rendererConfig->depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_SAMPLE_COUNT_1_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            shadowMapBufferModel->imageMemory);

        VkUtils::CreateImageView(logicalDevice, rendererConfig->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
            shadowMapBufferModel->image, shadowMapBufferModel->imageView);
    }

    void RenderPassShadowMaps::CreateFrameBuffers()
    {
        const std::vector<VkImageView> attachments =
        {
            shadowMapBufferModel->imageView
        };

        frameBuffers.resize(1);
        VkUtils::CreateFrameBuffer(logicalDevice, renderPass, swapChainData->extent, attachments, frameBuffers[0]);
    }

    void RenderPassShadowMaps::CreateSampler()
    {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

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

        auto createStatus = vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &sampler);
        CAssert::Check(createStatus == VK_SUCCESS, "Shadow map sampler can't be created");
    }

}
