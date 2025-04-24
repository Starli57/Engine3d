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
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Rendering/PipelinesCollection.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"
#include "EngineCore/Utilities/MathUtility.h"

namespace AVulkan
{
    class RenderPassColor : public IRenderPass
    {
    public:
        RenderPassColor(
            VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<VulkanConfiguration>& rendererConfig, const Ref<DescriptorsManager>& descriptorsManager,
            const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData, VkSampler& textureSampler,
            const Ref<PipelinesCollection>& pipelinesCollection, VkImageView& shadowMapImageView, VkSampler& shadowMapSampler);
        ~RenderPassColor() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex) override;

    protected:
        void CreateRenderPass(Ref<AVulkan::VulkanConfiguration> rendererConfig) override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
        void BeginRenderPass(VkCommandBuffer& commandBuffer, uint32_t imageIndex) override;

        VkSampler& textureSampler;
        VkImageView& shadowMapImageView;
        VkSampler& shadowMapSampler;

        //todo:replace to renderpass config
        Ref<PipelinesCollection> pipelinesCollection;
    };
}