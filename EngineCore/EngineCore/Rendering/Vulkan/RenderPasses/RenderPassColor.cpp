#include "EngineCore/Pch.h"
#include "RenderPassColor.h"
#include "EngineCore/Rendering/Vulkan/UniformBufferModel/UboLight.h"

namespace AVulkan
{
    RenderPassColor::RenderPassColor(
        VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<AVulkan::VulkanConfiguration> rendererConfig,
        Ref<Ecs> ecs, Ref<AssetsDatabaseVulkan> assetsDatabase, Ref<SwapChainData> swapChainData, Ref<Descriptors> descriptors, VkSampler& textureSampler,
        Ref<PipelinesCollection> pipelinesCollection, VkImageView& shadowMapImageView, VkSampler& shadowMapSampler) :
        IRenderPass(physicalDevice, logicalDevice, rendererConfig, ecs, assetsDatabase, swapChainData, descriptors), 
        textureSampler(textureSampler), pipelinesCollection(pipelinesCollection), 
        shadowMapImageView(shadowMapImageView), shadowMapSampler(shadowMapSampler)
    {
        spdlog::info("Create RenderPass Color");

        CreateRenderPass(rendererConfig);
        CreateDescriptorLayout(logicalDevice);
        CreatePipelines();
        CreateFrameBuffers();
    }

    RenderPassColor::~RenderPassColor()
    {
        spdlog::info("RenderPassColor::~RenderPassColor");

        for (auto frameBuffer : frameBuffers) VkUtils::DisposeFrameBuffer(logicalDevice, frameBuffer);
        frameBuffers.clear();

        GraphicsPipelineUtility pipelineUtility;
        for (auto pipeline : pipelines) pipelineUtility.Dispose(pipeline.second, logicalDevice);
        pipelines.clear();

        for (auto descriptor : passDescriptors)
        {
            VkUtils::DisposeBuffer(logicalDevice, descriptor->uboCamera->buffer, descriptor->uboCamera->bufferMemory);
            VkUtils::DisposeBuffer(logicalDevice, descriptor->uboLights->buffer, descriptor->uboLights->bufferMemory);
            VkUtils::DisposeBuffer(logicalDevice, descriptor->uboCameraViewProjection->buffer, descriptor->uboCameraViewProjection->bufferMemory);
        }
        passDescriptors.clear();

        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
        VkUtils::DisposeRenderPass(this->logicalDevice, renderPass);
    }

    void RenderPassColor::Render(VkCommandBuffer& commandBuffer, const uint16_t frame, const uint32_t imageIndex)
    {
        PositionComponent rendererPosition;
        UboViewProjectionComponent cameraProjection;
        UboViewProjectionComponent lightsProjection;
        UpdateRendererPositionAndProjection(rendererPosition, cameraProjection, lightsProjection);

        BeginRenderPass(commandBuffer, imageIndex);

        int i = 0;
        const auto entities = ecs->registry->view<UboModelComponent, MeshComponent, MaterialComponent>();
        for (const auto entity : entities)
        {
            int index = i * VulkanConfiguration::maxFramesInFlight + frame;
            auto [uboModelComponent, meshContainer, materialComponent] = entities.get<UboModelComponent, MeshComponent, MaterialComponent>(entity);

            if (meshContainer.meshIndex.has_value() == false) continue;

            int32_t meshIndex = meshContainer.meshIndex.value();
            UpdateUniformBuffer(index, materialComponent, rendererPosition, cameraProjection, lightsProjection);

            auto material = assetsDatabase->materials.at(materialComponent.materialIndex);
            auto pipeline = pipelines.at(material->pipelineId);

            VkUtils::BindPipeline(commandBuffer, pipeline);
            VkUtils::BindVertexAndIndexBuffers(commandBuffer, meshIndex, assetsDatabase);

            vkCmdPushConstants(commandBuffer, pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT,
                0, sizeof(glm::mat4), &uboModelComponent.model);

            auto descriptorSet = GetOrCreateDescriptorSet(index)->descriptorSet;
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 1, &descriptorSet, 0, nullptr);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(assetsDatabase->indexesCount.at(meshIndex)), 1, 0, 0, 0);
            i++;
        }

        VkUtils::EndRenderPass(commandBuffer);
    }

    void RenderPassColor::UpdateUniformBuffer(const uint32_t descriptorIndex, MaterialComponent& materialComponent,
        PositionComponent& cameraPosition, UboViewProjectionComponent& cameraProjection, UboViewProjectionComponent& lightProjection)
    {
        auto colorDescriptor = GetOrCreateDescriptorSet(descriptorIndex);

        memcpy(colorDescriptor->uboCameraViewProjection->bufferMapped, &cameraProjection, sizeof(UboViewProjectionComponent));

        auto lightEntries = ecs->registry->view<PositionComponent, RotationComponent, UboDiffuseLightComponent>();
        for (auto entity : lightEntries)
        {
            //todo: now color descriptor can handle only 1 light entry, need to extend
            auto& position = lightEntries.get<PositionComponent>(entity).position;
            auto& rotation = lightEntries.get<RotationComponent>(entity).rotation;
            glm::mat4 viewProjection = lightProjection.proj * lightProjection.view;

            glm::vec3 direction;
            CalculateDirection(&direction, rotation + glm::vec3(0,90,0));
            Normalize(&direction);

            UboLight uboLight = UboLight(viewProjection, position, direction);
            memcpy(colorDescriptor->uboLights->bufferMapped, &uboLight, sizeof(UboLight));
        }

        memcpy(colorDescriptor->uboCamera->bufferMapped, &cameraPosition, sizeof(PositionComponent));

        UpdateDescriptorSet(colorDescriptor, materialComponent.materialIndex);
    }

#pragma optimize("", off)
    void RenderPassColor::UpdateDescriptorSet(Ref<ColorDescriptor> colorDescriptor, uint32_t materialIndex)
    {
        auto material = assetsDatabase->materials.at(materialIndex);

        VkImageView diffuseImageView = nullptr;
        if (material->diffuse.has_value())
        {
            diffuseImageView = assetsDatabase->imagesViews.at(material->diffuse.value());
        }

        VkImageView specularImageView = nullptr;
        if (material->specular.has_value())
        {
            specularImageView = assetsDatabase->imagesViews.at(material->specular.value());
        }

        VkImageView normalMapImageView = nullptr;
        if (material->normalMap.has_value())
        {
            normalMapImageView = assetsDatabase->imagesViews.at(material->normalMap.value());
        }

        VkImageView alphaMapImageView = nullptr;
        if (material->alphaMap.has_value())
        {
            alphaMapImageView = assetsDatabase->imagesViews.at(material->alphaMap.value());
        }

        VkDescriptorBufferInfo viewProjectionDescriptorInfo{};
        viewProjectionDescriptorInfo.buffer = colorDescriptor->uboCameraViewProjection->buffer;
        viewProjectionDescriptorInfo.range = sizeof(UboViewProjectionComponent);
        viewProjectionDescriptorInfo.offset = 0;

        VkDescriptorBufferInfo lightDescriptorInfo{};
        lightDescriptorInfo.buffer = colorDescriptor->uboLights->buffer;
        lightDescriptorInfo.range = sizeof(UboLight);
        lightDescriptorInfo.offset = 0;

        VkDescriptorBufferInfo cameraDescriptorInfo{};
        cameraDescriptorInfo.buffer = colorDescriptor->uboCamera->buffer;
        cameraDescriptorInfo.range = sizeof(glm::vec3);
        cameraDescriptorInfo.offset = 0;

        VkDescriptorImageInfo shadowMapInfo{};
        shadowMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        shadowMapInfo.imageView = shadowMapImageView;
        shadowMapInfo.sampler = shadowMapSampler;

        VkDescriptorImageInfo diffuseImageInfo{};
        diffuseImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        diffuseImageInfo.imageView = diffuseImageView;
        diffuseImageInfo.sampler = textureSampler;

        VkDescriptorImageInfo specularImageInfo{};
        specularImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        specularImageInfo.imageView = specularImageView;
        specularImageInfo.sampler = textureSampler;

        VkDescriptorImageInfo normalImageInfo{};
        normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        normalImageInfo.imageView = normalMapImageView;
        normalImageInfo.sampler = textureSampler;

        VkDescriptorImageInfo alphaImageInfo{};
        alphaImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        alphaImageInfo.imageView = alphaMapImageView;
        alphaImageInfo.sampler = textureSampler;

        std::array<VkWriteDescriptorSet, 8> descriptorWrites{};

        WriteDescriptorSet(descriptorWrites[0], colorDescriptor->descriptorSet, 0, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &viewProjectionDescriptorInfo);

        WriteDescriptorSet(descriptorWrites[1], colorDescriptor->descriptorSet, 1, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &lightDescriptorInfo);

        WriteDescriptorSet(descriptorWrites[2], colorDescriptor->descriptorSet, 2, 0, 1,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &cameraDescriptorInfo);

        WriteDescriptorSet(descriptorWrites[3], colorDescriptor->descriptorSet, 3, 0, 1,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &shadowMapInfo, nullptr);

        WriteDescriptorSet(descriptorWrites[4], colorDescriptor->descriptorSet, 4, 0, 1,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &diffuseImageInfo, nullptr);

        WriteDescriptorSet(descriptorWrites[5], colorDescriptor->descriptorSet, 5, 0, 1,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &specularImageInfo, nullptr);

        WriteDescriptorSet(descriptorWrites[6], colorDescriptor->descriptorSet, 6, 0, 1,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &normalImageInfo, nullptr);

        WriteDescriptorSet(descriptorWrites[7], colorDescriptor->descriptorSet, 7, 0, 1,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &alphaImageInfo, nullptr);

        vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
#pragma optimize("", on)

    void RenderPassColor::UpdateRendererPositionAndProjection(PositionComponent& positionComponent,
        UboViewProjectionComponent& cameraProjection, UboViewProjectionComponent& lightProjection)
    {
        auto cameraEntries = ecs->registry->view<PositionComponent, CameraComponent, UboViewProjectionComponent>();
        if (cameraEntries.begin() != cameraEntries.end())
        {
            auto firstCamera = cameraEntries.front();
            positionComponent = cameraEntries.get<PositionComponent>(firstCamera);
            cameraProjection = cameraEntries.get<UboViewProjectionComponent>(firstCamera);
        }

        auto lightsEntries = ecs->registry->view<PositionComponent, UboDiffuseLightComponent, UboViewProjectionComponent>();
        if (lightsEntries.begin() != lightsEntries.end())
        {
            auto firstLight = lightsEntries.front();
            lightProjection = lightsEntries.get<UboViewProjectionComponent>(firstLight);
        }
    }

    Ref<ColorDescriptor> RenderPassColor::GetOrCreateDescriptorSet(uint32_t index)
    {
        auto diff = static_cast<int32_t>(index) - static_cast<int32_t>(passDescriptors.size()) + 1i32;
        for (int32_t i = 0; i < diff; i++) CreateDescriptorSet();
        return passDescriptors.at(index);
    }

    void RenderPassColor::CreateDescriptorSet()
    {
        Ref<ColorDescriptor> colorDescriptor = CreateRef<ColorDescriptor>();
        descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout, colorDescriptor->descriptorSet);
        colorDescriptor->uboCameraViewProjection = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboViewProjectionComponent));
        colorDescriptor->uboLights = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(UboLight));
        colorDescriptor->uboCamera = VkUtils::CreateUniformBuffer(logicalDevice, physicalDevice, sizeof(PositionComponent));
        passDescriptors.push_back(colorDescriptor);
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

    void RenderPassColor::CreateDescriptorLayout(VkDevice& logicalDevice)
    {
        auto uboViewProjectionLayout = descriptors->DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        auto uboLightLayout = descriptors->DescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        auto uboCameraLayout = descriptors->DescriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1);
        auto shadowMap = descriptors->DescriptorSetLayoutBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        auto diffuseMap = descriptors->DescriptorSetLayoutBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        auto specularMap = descriptors->DescriptorSetLayoutBinding(5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        auto normalMap = descriptors->DescriptorSetLayoutBinding(6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
        auto alphaMap = descriptors->DescriptorSetLayoutBinding(7, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);

        std::vector bindings = { uboViewProjectionLayout, uboLightLayout, uboCameraLayout, shadowMap, diffuseMap, specularMap, normalMap, alphaMap };
        descriptors->CreateLayout(logicalDevice, bindings, descriptorSetLayout);
    }

    void RenderPassColor::CreatePipelines()
    {
        pipelines.reserve(pipelinesCollection->pipelinesConfigs.size());

        for (auto config : pipelinesCollection->pipelinesConfigs)
        {
            GraphicsPipelineUtility pipelineUtility;
            auto pipeline = pipelineUtility.Create(
                config.second, logicalDevice, renderPass,
                swapChainData->extent, descriptorSetLayout, rendererConfig->msaa);

            pipelines.emplace(config.first, pipeline);
        }
    }

    void RenderPassColor::CreateRenderPass(Ref<AVulkan::VulkanConfiguration> rendererConfig)
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