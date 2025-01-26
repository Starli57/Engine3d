#pragma once

#include <GLFW/glfw3.h>

#include "IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/RenderPassUtility.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/DescriptorsManager.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/ShadowMapsDescriptor.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/TextureSamplerUtility.h"
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

namespace AVulkan
{
    class RenderPassShadowMaps : public IRenderPass
    {
    public:
        RenderPassShadowMaps(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig,
                             const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData, const Ref<DescriptorsManager>& descriptorsManager);
        ~RenderPassShadowMaps() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) override;

        Ref<ImageModel> GetImageBuffer() { return shadowMapBufferModel; }
        VkSampler& GetSampler() { return sampler; }

        std::vector<Ref<ShadowMapsDescriptor>> GetPassDescriptors() { return passDescriptors; }

    protected:
        void CreateRenderPass(Ref<VulkanConfiguration> rendererConfig) override;
        void CreateDescriptorLayout(VkDevice& logicalDevice) override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
        void CreateShadowMapBuffer();

        void BeginRenderPass(VkCommandBuffer& commandBuffer, uint32_t imageIndex) override;
        void UpdateUniformBuffer(uint32_t descriptorIndex, const UboViewProjectionComponent& projection);
        void UpdateRendererPositionAndProjection(PositionComponent& positionComponent, UboViewProjectionComponent& projectionComponent) const;

        Ref<ShadowMapsDescriptor> GetOrCreateDescriptorSet(uint32_t index);
        void CreateDescriptorSet();
        void UpdateDescriptorSet(const Ref<ShadowMapsDescriptor>& descriptor) const;

        void CreateSampler();

        std::vector<Ref<ShadowMapsDescriptor>> passDescriptors;
        Ref<ImageModel> shadowMapBufferModel;
        VkSampler sampler;
    };
}