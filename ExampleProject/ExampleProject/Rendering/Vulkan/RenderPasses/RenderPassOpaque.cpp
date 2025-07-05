#include "EngineCore/Pch.h"
#include "RenderPassOpaque.h"

#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboLight.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "Utility/DrawEntity.h"

namespace AVulkan
{
    RenderPassOpaque::RenderPassOpaque(
        Ref<VulkanContext> vulkanContext, const Ref<DescriptorsManager>& descriptorsManager,
        const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData) :
        IRenderPass(vulkanContext, descriptorsManager, ecs, assetsDatabase, swapChainData)
    {
        spdlog::info("Create RenderPass Opaque");
        RenderPassOpaque::CreateRenderPass();
        RenderPassOpaque::CreatePipelines();
        RenderPassOpaque::CreateFrameBuffers();
    }

    RenderPassOpaque::~RenderPassOpaque()
    {
        spdlog::info("RenderPassOpaque::~RenderPassOpaque");

        for (auto frameBuffer : frameBuffers) VkUtils::DisposeFrameBuffer(vulkanContext->logicalDevice, frameBuffer);
        frameBuffers.clear();

        GraphicsPipelineUtility pipelineUtility;
        for (const auto& pipeline : pipelines) pipelineUtility.Dispose(pipeline.second, vulkanContext->logicalDevice);
        pipelines.clear();
        
        VkUtils::DisposeRenderPass(vulkanContext->logicalDevice, renderPass);
    }

    void RenderPassOpaque::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter)
    {
		Profiler::GetInstance().BeginSample("RenderPassOpaque");
        BeginRenderPass(commandBuffer, imageIndex, 2, 1);
        
        for (const auto& drawEntity : ecs->allEntities)
        {
            if (!filter(drawEntity)) continue;
            
            MeshComponent meshComponent;
            MaterialComponent materialComponent;
            UboModelComponent uboModelComponent;
            if (!drawEntity->TryGetComponent(meshComponent)) continue;
            if (!drawEntity->TryGetComponent(materialComponent)) continue;
            if (!drawEntity->TryGetComponent(uboModelComponent)) continue;;
            if (meshComponent.meshIndex.has_value() == false) continue;

            const auto material = assetsDatabase->materials.at(materialComponent.materialIndex);
                
            const int32_t meshIndex = meshComponent.meshIndex.value();

            const auto pipeline = pipelines.at(material->pipelineId);
            VkUtils::BindPipeline(commandBuffer, pipeline);
            VkUtils::BindVertexAndIndexBuffers(commandBuffer, meshIndex, assetsDatabase);

            vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT,
                0, sizeof(glm::mat4), &uboModelComponent.model);

            auto descriptorSets = std::vector<VkDescriptorSet>();
            descriptorSets.resize(3);
            descriptorSets.at(0) = descriptorsManager->GetDescriptorSetFrame(frame);
            descriptorSets.at(1) = descriptorsManager->GetDescriptorSetOpaqueMaterial(frame, materialComponent.materialIndex);
            descriptorSets.at(2) = descriptorsManager->GetDescriptorSetShadowMap(frame);
            
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0,
                static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, assetsDatabase->indexesCount.at(meshIndex), 1, 0, 0, 0);
		    Profiler::GetInstance().AddDrawCall();
        }
        
        VkUtils::EndRenderPass(commandBuffer);
		Profiler::GetInstance().EndSample();
    }

    void RenderPassOpaque::CreateFrameBuffers()
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

            VkUtils::CreateFrameBuffer(vulkanContext->logicalDevice, renderPass, swapChainData->extent.width, swapChainData->extent.height,
                attachments, frameBuffers[i]);
        }
    }

    void RenderPassOpaque::CreatePipelines()
    {
        pipelines.reserve(vulkanContext->pipelinesCollection->pipelinesConfigs.size());

        auto descriptorSetLayouts = std::vector<VkDescriptorSetLayout>();
        descriptorSetLayouts.resize(3);
        descriptorSetLayouts.at(0) = descriptorsManager->GetDescriptorSetLayoutFrame();
        descriptorSetLayouts.at(1) = descriptorsManager->GetDescriptorSetLayoutOpaqueMaterial();
        descriptorSetLayouts.at(2) = descriptorsManager->GetDescriptorSetLayoutShadowMap();
        
        for (const auto& config : vulkanContext->pipelinesCollection->pipelinesConfigs)
        {
            GraphicsPipelineUtility pipelineUtility;
            auto pipeline = pipelineUtility.Create(config.second, vulkanContext->logicalDevice, renderPass,
                swapChainData->extent, descriptorSetLayouts, vulkanContext->msaa);

            pipelines.emplace(config.first, pipeline);
        }
    }

    void RenderPassOpaque::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = vulkanContext->imageFormat;
        colorAttachment.samples = vulkanContext->msaa;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = vulkanContext->imageFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = vulkanContext->depthFormat;
        depthAttachment.samples = vulkanContext->msaa;
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

        const std::vector<VkAttachmentDescription> attachments = { colorAttachment, colorAttachmentResolve, depthAttachment };

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        renderPass = VkUtils::CreateRenderPass(vulkanContext, attachments, subpass);
    }
}
