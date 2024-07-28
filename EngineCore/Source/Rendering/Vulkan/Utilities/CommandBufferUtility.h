#pragma once

#include <GLFW/glfw3.h>

#include <unordered_map>

#include "AssetsDatabase.h"

#include "Rendering/Vulkan/MeshVulkan.h"
#include "Rendering/Vulkan/TextureVulkan.h"
#include "Rendering/Vulkan/PipelineVulkan.h"
#include "Rendering/Vulkan/MaterialVulkan.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"

#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/Components/MeshComponent.h"
#include "EngineShared/Components/MaterialComponent.h"
#include "EngineShared/Components/UboModelComponent.h"

namespace VkUtils
{
	void AllocateCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount);
	void FreeCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers);

	void BeginCommandBuffer(VkCommandBuffer& commandBuffer);
	void EndCommandBuffer(VkCommandBuffer& commandBuffer);

	void BeginRenderPass(VkFramebuffer& frameBuffer, VkRenderPass& renderPass, VkCommandBuffer& commandBuffer, VkExtent2D& vkExtent);
	void EndRenderPass(VkCommandBuffer& commandBuffer);

	void RecordCommandBuffer(Ref<Ecs> ecs, Ref<AssetsDatabase> assetsDatabase, Ref<AVulkan::Descriptors> descriptors, uint16_t frame,
		VkCommandBuffer& commandBuffer, std::unordered_map<std::string, Ref<PipelineVulkan>>& pipelines);
}