#pragma once

#include <GLFW/glfw3.h>
#include <entt.hpp>

#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Rendering/Vulkan/Entities/TextureVulkan.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "SharedLib/Components/UboModelComponent.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Allocate(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount) const;
		
		void Begin(VkCommandBuffer& commandBuffer) const;
		void End(VkCommandBuffer& commandBuffer) const;

		void BeginRenderPass(VkFramebuffer& frameBuffer, VkRenderPass& renderPass, VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent) const;
		void EndRenderPass(VkCommandBuffer& commandBuffer) const;

		void Record(Ref<entt::registry> ecs, Ref<Descriptors> descriptors, uint16_t frame,
			VkCommandBuffer& commandBuffer, GraphicsPipeline& pipeline) const;
	};
}