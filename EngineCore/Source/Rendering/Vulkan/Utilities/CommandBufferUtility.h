#pragma once

#include <GLFW/glfw3.h>

#include <unordered_map>

#include "Rendering/Vulkan/MeshVulkan.h"
#include "Rendering/Vulkan/TextureVulkan.h"
#include "Rendering/Vulkan/VulkanPipeline.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"

#include "Components/MaterialComponent.h"

#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/Components/MeshComponent.h"
#include "EngineShared/Components/UboModelComponent.h"

namespace VkUtils
{
	void AllocateCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount);
	void FreeCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers);

	void BeginCommandBuffer(VkCommandBuffer& commandBuffer);
	void EndCommandBuffer(VkCommandBuffer& commandBuffer);

	void BeginRenderPass(VkFramebuffer& frameBuffer, VkRenderPass& renderPass, VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent);
	void EndRenderPass(VkCommandBuffer& commandBuffer);

	void RecordCommandBuffer(Ref<Ecs> ecs, Ref<AVulkan::Descriptors> descriptors, uint16_t frame,
		VkCommandBuffer& commandBuffer, std::unordered_map<std::string, Ref<VulkanPipeline>>& pipelines);
}