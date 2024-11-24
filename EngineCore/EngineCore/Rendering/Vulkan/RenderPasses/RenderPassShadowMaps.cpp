#include "EngineCore/Pch.h"
#include "RenderPassShadowMaps.h"

namespace AVulkan
{
    RenderPassShadowMaps::RenderPassShadowMaps(
        VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<AVulkan::VulkanConfiguration> rendererConfig,
        Ref<Ecs> ecs, Ref<AssetsDatabase> assetsDatabase, Ref<SwapChainData> swapChainData, Ref<Descriptors> descriptors) :
        IRenderPass(physicalDevice, logicalDevice, rendererConfig, ecs, assetsDatabase, swapChainData, descriptors)
    {
        spdlog::info("Create RenderPass ShadowMaps");

        CreateRenderPass(rendererConfig);
        CreateDescriptorLayout(logicalDevice);
        CreatePipelines();
        CreateShadowMapBuffer();
        CreateFrameBuffers();
        CreateSampler();    
    }

    RenderPassShadowMaps::~RenderPassShadowMaps()
    {
        spdlog::info("RenderPassShadowMaps::~RenderPassShadowMaps"); 
        vkDestroySampler(logicalDevice, sampler, nullptr);

        VkUtils::DisposeFrameBuffer(logicalDevice, frameBuffers);
        frameBuffers.clear();

        VkUtils::DisposeImageModel(logicalDevice, shadowMapBufferModel);

        for(auto pipelinePair : pipelines)
            GraphicsPipelineUtility().Dispose(pipelinePair.second, logicalDevice);
        pipelines.clear();

        for (auto descriptor : passDescriptors)
        {
            VkUtils::DisposeBuffer(logicalDevice, descriptor->uboLightsViewProjection->buffer, descriptor->uboLightsViewProjection->bufferMemory);
        }
        passDescriptors.clear();

        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

        VkUtils::DisposeRenderPass(this->logicalDevice, renderPass);
    }

    void RenderPassShadowMaps::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex)
    {
        PositionComponent rendererPosition{};
        UboViewProjectionComponent rendererProjection{};
        UpdateRendererPositionAndProjection(rendererPosition, rendererProjection);

        BeginRenderPass(commandBuffer, imageIndex);

        int i = 0;
        const auto entities = ecs->registry->view<UboModelComponent, MeshComponent, MaterialComponent>();
        for (const auto entity : entities)
        {
            int index = i * VulkanConfiguration::maxFramesInFlight + frame;
            auto [uboModelComponent, meshContainer, materialComponent] = entities.get<UboModelComponent, MeshComponent, MaterialComponent>(entity);

            if (meshContainer.meshIndex.has_value() == false) continue;

            UpdateUniformBuffer(index, rendererProjection);

            auto meshVulkan = std::static_pointer_cast<AVulkan::MeshVulkan> (assetsDatabase->GetMesh(meshContainer.meshIndex.value()));
            auto pipeline = pipelines.at("shadowPass");

            VkUtils::BindPipeline(commandBuffer, pipeline);
            VkUtils::BindVertexAndIndexBuffers(commandBuffer, meshVulkan, assetsDatabase);

            vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT,
                0, sizeof(glm::mat4), &uboModelComponent.model);

            auto descriptorSet = GetOrCreateDescriptorSet(index)->descriptorSet;
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 1, &descriptorSet, 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(meshVulkan->GetIndicesCount()), 1, 0, 0, 0);
            i++;
        }

        VkUtils::EndRenderPass(commandBuffer);
    }

    void RenderPassShadowMaps::UpdateRendererPositionAndProjection(PositionComponent& positionComponent, UboViewProjectionComponent& projectionComponent)
    {
        auto viewProjectionEntries = ecs->registry->view<PositionComponent, UboDiffuseLightComponent, UboViewProjectionComponent>();
        if (viewProjectionEntries.begin() == viewProjectionEntries.end()) return;

        auto firstLight = viewProjectionEntries.front();
        positionComponent = viewProjectionEntries.get<PositionComponent>(firstLight);
        projectionComponent = viewProjectionEntries.get<UboViewProjectionComponent>(firstLight);
    }

    void RenderPassShadowMaps::UpdateUniformBuffer(const uint32_t descriptorIndex, UboViewProjectionComponent& projection)
    {
        auto colorDescriptor = GetOrCreateDescriptorSet(descriptorIndex);

        memcpy(colorDescriptor->uboLightsViewProjection->bufferMapped, &projection, sizeof(UboViewProjectionComponent));

        UpdateDescriptorSet(colorDescriptor);
    }

    void RenderPassShadowMaps::UpdateDescriptorSet(Ref<ShadowMapsDescriptor> descriptor)
    {
        VkDescriptorBufferInfo viewProjectionDescriptorInfo{};
        viewProjectionDescriptorInfo.buffer = descriptor->uboLightsViewProjection->buffer;
        viewProjectionDescriptorInfo.range = sizeof(UboViewProjectionComponent);
        viewProjectionDescriptorInfo.offset = 0;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        WriteDescriptorSet(descriptorWrites[0], descriptor->descriptorSet, 0, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &viewProjectionDescriptorInfo);

        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    void RenderPassShadowMaps::BeginRenderPass(VkCommandBuffer& commandBuffer, const uint32_t imageIndex)
    {
        std::vector<VkClearValue> clearValues;
        clearValues.resize(1);

        clearValues[0].depthStencil.depth = 1.0f;

        VkUtils::BeginRenderPass(clearValues, frameBuffers[0], renderPass, commandBuffer, swapChainData->extent);
    }

    Ref<ShadowMapsDescriptor> RenderPassShadowMaps::GetOrCreateDescriptorSet(uint32_t index)
    {
        int diff = index - passDescriptors.size() + 1;
        for (int i = 0; i < diff; i++) CreateDescriptorSet();
        return passDescriptors.at(index);
    }

    void RenderPassShadowMaps::CreateDescriptorSet()
    {
        Ref<ShadowMapsDescriptor> descriptor = CreateRef<ShadowMapsDescriptor>();
        descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout, descriptor->descriptorSet);
        descriptor->uboLightsViewProjection = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboViewProjectionComponent));
        passDescriptors.push_back(descriptor);
    }

    void RenderPassShadowMaps::CreateRenderPass(Ref<AVulkan::VulkanConfiguration> rendererConfig)
    {
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = rendererConfig->depthFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 0;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::vector<VkAttachmentDescription> attachments = { depthAttachment };

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 0;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        renderPass = VkUtils::CreateRenderPass(physicalDevice, logicalDevice, rendererConfig, attachments, subpass);
    }

    void RenderPassShadowMaps::CreateDescriptorLayout(VkDevice& logicalDevice)
    {
        std::vector bindings = { descriptors->DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1) };
        descriptors->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void RenderPassShadowMaps::CreatePipelines()
    {
        auto shadowPassPipelineConfig = CreateRef<VulkanPipelineConfig>();
        shadowPassPipelineConfig->pipelineName = "shadowPass";
        shadowPassPipelineConfig->fragShaderPath = "../ExampleProject/Shaders/ShadowPassFrag.spv";
        shadowPassPipelineConfig->vertShaderPath = "../ExampleProject/Shaders/ShadowPassVert.spv";
        shadowPassPipelineConfig->polygonMode = VK_POLYGON_MODE_FILL;
        shadowPassPipelineConfig->cullMode = VK_CULL_MODE_BACK_BIT;
        shadowPassPipelineConfig->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

        auto shadowPassPipeline = GraphicsPipelineUtility().Create(
            shadowPassPipelineConfig, logicalDevice, renderPass,
            swapChainData->extent, descriptorSetLayout, VK_SAMPLE_COUNT_1_BIT);
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
        std::vector<VkImageView> attachments =
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