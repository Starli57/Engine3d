#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Core/IRenderPass.h"
#include "EngineCore/Rendering/Vulkan/Utilities/UniformBufferVulkanUtility.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Ref.h"

namespace ClientVulkanApi
{
    class RenderPassOpaque : public IRenderPass
    {
    public:
        RenderPassOpaque(VulkanApi::VulkanContext* vulkanContext, const Ref<RenderPassContext>& renderPassContext);
        ~RenderPassOpaque() override;

        void Render(VkCommandBuffer& commandBuffer, uint16_t frame, uint32_t imageIndex, std::function<bool(const Ref<Entity>& entity)> filter) override;
        void RenderEntity(const DrawEntity& drawEntity, const VkCommandBuffer& commandBuffer, const uint16_t frame) const;

    protected:
        void CreateRenderPass() override;
        void CreatePipelines() override;
        void CreateFrameBuffers() override;
    };
}