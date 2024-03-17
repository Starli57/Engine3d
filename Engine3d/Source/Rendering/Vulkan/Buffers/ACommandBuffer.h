#pragma once

#include <GLFW/glfw3.h>
#include <entt.hpp>

#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Systems/MeshContainer.h"
#include "SharedLib/Components/UboModelComponent.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, SwapChainData& swapChainData, int buffersCount) const;
		void Record(Ref<entt::registry> ecs, uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			SwapChainData& swapChainData, GraphicsPipeline& pipeline) const;
	};
}