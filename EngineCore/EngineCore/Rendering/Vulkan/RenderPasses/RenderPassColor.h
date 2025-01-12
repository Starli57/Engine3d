#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <string>
#include <vector>

#include "IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Rendering/Vulkan/Descriptors.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/ColorDescriptor.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Rendering/PipelinesCollection.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Utilities/MathUtility.h"

namespace AVulkan
{
    class RenderPassColor : public IRenderPass
    {
    public:
        RenderPassColor(
            VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<VulkanConfiguration> rendererConfig,
            Ref<Ecs> ecs, Ref<AssetsDatabaseVulkan> assetsDatabase, Ref<SwapChainData> swapChainData, Ref<Descriptors> descriptors, VkSampler& textureSampler,
            Ref<PipelinesCollection> pipelinesCollection, VkImageView& shadowMapImageView, VkSampler& shadowMapSampler);
        ~RenderPassColor() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) override;

    protected:
        void CreateRenderPass(Ref<AVulkan::VulkanConfiguration> rendererConfig) override;
        void CreateDescriptorLayout(VkDevice& logicalDevice) override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;

        void BeginRenderPass(VkCommandBuffer& commandBuffer, const uint32_t imageIndex) override;
        void UpdateUniformBuffer(const uint32_t descriptorIndex, MaterialComponent& materialComponent,
            PositionComponent& cameraPosition, UboViewProjectionComponent& cameraProjection, UboViewProjectionComponent& lightProjection);
        void UpdateRendererPositionAndProjection(PositionComponent& positionComponent, 
            UboViewProjectionComponent& cameraProjection, UboViewProjectionComponent& lightProjection);

        Ref<ColorDescriptor> GetOrCreateDescriptorSet(uint32_t index);
        void CreateDescriptorSet();
        void UpdateDescriptorSet(Ref<ColorDescriptor> colorDescriptor, uint32_t materialIndex);

        VkSampler& textureSampler;
        std::vector<Ref<ColorDescriptor>> passDescriptors;

        VkImageView& shadowMapImageView;
        VkSampler& shadowMapSampler;

        //todo:replace to renderpass config
        Ref<PipelinesCollection> pipelinesCollection;
    };
}