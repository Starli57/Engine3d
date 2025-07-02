#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <string>
#include <vector>

#include "IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Rendering/PipelinesCollection.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Descriptors/DescriptorsManager.h"
#include "EngineCore/Utilities/MathUtility.h"

namespace AVulkan
{
    class RenderPassOpaque : public IRenderPass
    {
    public:
        RenderPassOpaque(
            Ref<VulkanContext> vulkanContext, const Ref<DescriptorsManager>& descriptorsManager,
            const Ref<Ecs>& ecs, const Ref<AssetsDatabaseVulkan>& assetsDatabase, const Ref<SwapChainData>& swapChainData);
        ~RenderPassOpaque() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) override;

    protected:
        void CreateRenderPass() override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
    };
}