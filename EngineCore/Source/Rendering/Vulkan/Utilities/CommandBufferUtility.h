#pragma once

#include <GLFW/glfw3.h>

#include "EngineShared/Ref.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Rendering/Vulkan/Entities/TextureVulkan.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/Components/UboModelComponent.h"

namespace VkUtils
{
	void AllocateCommandBuffer(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount);
		
	void BeginCommandBuffer(VkCommandBuffer& commandBuffer);
	void EndCommandBuffer(VkCommandBuffer& commandBuffer);

	void BeginRenderPass(VkFramebuffer& frameBuffer, VkRenderPass& renderPass, VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent);
	void EndRenderPass(VkCommandBuffer& commandBuffer);

	void RecordCommandBuffer(Ref<Ecs> ecs, Ref<AVulkan::Descriptors> descriptors, uint16_t frame,
		VkCommandBuffer& commandBuffer, AVulkan::GraphicsPipeline& pipeline);
}