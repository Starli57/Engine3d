#include "EngineCore/Pch.h"
#include "RenderPassColor.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboLight.h"
#include "Utility/DrawEntity.h"

namespace AVulkan
{
    RenderPassColor::RenderPassColor(
        VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig, const Ref<DescriptorsManager>& descriptorsManager,
        const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData, VkSampler& textureSampler,
        const Ref<PipelinesCollection>& pipelinesCollection, VkImageView& shadowMapImageView, VkSampler& shadowMapSampler) :
        IRenderPass(physicalDevice, logicalDevice, rendererConfig, descriptorsManager, ecs, assetsDatabase, swapChainData), 
        textureSampler(textureSampler), shadowMapImageView(shadowMapImageView), 
        shadowMapSampler(shadowMapSampler), pipelinesCollection(pipelinesCollection)
    {
        spdlog::info("Create RenderPass Color");
        RenderPassColor::CreateRenderPass(rendererConfig);
        RenderPassColor::CreatePipelines();
        RenderPassColor::CreateFrameBuffers();
    }

    RenderPassColor::~RenderPassColor()
    {
        spdlog::info("RenderPassColor::~RenderPassColor");

        for (auto frameBuffer : frameBuffers) VkUtils::DisposeFrameBuffer(logicalDevice, frameBuffer);
        frameBuffers.clear();

        GraphicsPipelineUtility pipelineUtility;
        for (const auto pipeline : pipelines) pipelineUtility.Dispose(pipeline.second, logicalDevice);
        pipelines.clear();
        
        VkUtils::DisposeRenderPass(this->logicalDevice, renderPass);
    }

    void RenderPassColor::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex)
    {
		Profiler::GetInstance().BeginSample("RenderPassColor");
        
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
        
        // std::ranges::sort(drawEntities, [rendererPosition](const DrawEntity& a, const DrawEntity& b)
        // {
        //     return distance(a.positionComponent->position, rendererPosition.position) < distance(b.positionComponent->position, rendererPosition.position);
        // });
        
        int i = 0;
        for (const auto drawEntity : drawEntities)
        {
            const int index = i * VulkanConfiguration::maxFramesInFlight + frame;
            i++;

            if (drawEntity.meshComponent->meshIndex.has_value() == false) continue;

            const auto material = assetsDatabase->materials.at(drawEntity.materialComponent->materialIndex);
                
            const int32_t meshIndex = drawEntity.meshComponent->meshIndex.value();

            const auto pipeline = pipelines.at(material->pipelineId);
            VkUtils::BindPipeline(commandBuffer, pipeline);
            VkUtils::BindVertexAndIndexBuffers(commandBuffer, meshIndex, assetsDatabase);

            vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT,
                0, sizeof(glm::mat4), &drawEntity.uboModelComponent->model);

            auto descriptorSets = std::vector<VkDescriptorSet>();
            descriptorSets.resize(3);
            descriptorSets.at(0) = descriptorsManager->GetDescriptorSetFrame(frame);
            descriptorSets.at(1) = descriptorsManager->GetDescriptorSetOpaqueMaterial(frame, drawEntity.materialComponent->materialIndex);
            descriptorSets.at(2) = descriptorsManager->GetDescriptorSetShadowMap(frame);
            
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0,
                static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, assetsDatabase->indexesCount.at(meshIndex), 1, 0, 0, 0);
        }
        
        VkUtils::EndRenderPass(commandBuffer);
		Profiler::GetInstance().EndSample("RenderPassColor");
    }

    void RenderPassColor::CreateFrameBuffers()
    {
        frameBuffers.resize(swapChainData->imagesCount);

        for (size_t i = 0; i < swapChainData->imagesCount; i++)
        {
            std::vector<VkImageView> attachments =
            {
                swapChainData->msaaColorSample->imageView,
                swapChainData->imageViews[i],
                swapChainData->msaaDepthSample->imageView

            };

            VkUtils::CreateFrameBuffer(logicalDevice, renderPass, swapChainData->extent, attachments, frameBuffers[i]);
        }
    }

    void RenderPassColor::CreatePipelines()
    {
        pipelines.reserve(pipelinesCollection->pipelinesConfigs.size());

        auto descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();
        descriptorSetLayouts.resize(3);
        descriptorSetLayouts.at(0) = descriptorsManager->GetDescriptorSetLayoutFrame();
        descriptorSetLayouts.at(1) = descriptorsManager->GetDescriptorSetLayoutOpaqueMaterial();
        descriptorSetLayouts.at(2) = descriptorsManager->GetDescriptorSetLayoutShadowMap();
        
        for (auto config : pipelinesCollection->pipelinesConfigs)
        {
            GraphicsPipelineUtility pipelineUtility;
            auto pipeline = pipelineUtility.Create(
                config.second, logicalDevice, renderPass,
                swapChainData->extent, descriptorSetLayouts, rendererConfig->msaa);

            pipelines.emplace(config.first, pipeline);
        }
    }

    void RenderPassColor::CreateRenderPass(Ref<VulkanConfiguration> rendererConfig)
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = rendererConfig->imageFormat;
        colorAttachment.samples = rendererConfig->msaa;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = rendererConfig->imageFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = rendererConfig->depthFormat;
        depthAttachment.samples = rendererConfig->msaa;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 1;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 2;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::vector<VkAttachmentDescription> attachments = { colorAttachment, colorAttachmentResolve, depthAttachment };

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        renderPass = VkUtils::CreateRenderPass(physicalDevice, logicalDevice, rendererConfig, attachments, subpass);
    }

    void RenderPassColor::BeginRenderPass(VkCommandBuffer& commandBuffer, const uint32_t imageIndex)
    {
        std::vector<VkClearValue> clearValues;
        clearValues.resize(3);

        clearValues[0].color = { 0.015f, 0.015f, 0.04f, 1.0f };
        clearValues[1].color = { 0.015f, 0.015f, 0.04f, 1.0f };
        clearValues[2].depthStencil.depth = 1.0f;

        VkUtils::BeginRenderPass(clearValues, frameBuffers[imageIndex], renderPass, commandBuffer, swapChainData->extent);
    }
}
