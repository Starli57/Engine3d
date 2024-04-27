#pragma once

#include <GLFW/glfw3.h>
#include <entt.hpp>

#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Rendering/Vulkan/Entities/TextureVulkan.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Systems/MeshComponent.h"
#include "Systems/MaterialComponent.h"
#include "SharedLib/Components/UboModelComponent.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount) const;
		void Record(Ref<entt::registry> ecs, Ref<Descriptors> descriptors, uint16_t frame, VkFramebuffer& frameBuffer, VkRenderPass& renderPass,
			std::vector<VkCommandBuffer>& commandBuffers, GraphicsPipeline& pipeline, VkExtent2D& vkExtent) const;
	};
}