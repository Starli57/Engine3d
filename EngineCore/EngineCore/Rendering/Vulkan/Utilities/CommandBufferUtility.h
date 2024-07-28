#pragma once

#include <GLFW/glfw3.h>

#include <unordered_map>

#include "EngineCore/AssetsDatabase.h"

#include "EngineCore/Rendering/Vulkan/MeshVulkan.h"
#include "EngineCore/Rendering/Vulkan/TextureVulkan.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/MaterialVulkan.h"
#include "EngineCore/Rendering/Vulkan/Descriptors.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"

#include "EngineCore/Ref.h"
#include "EngineCore/Ecs.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/UboModelComponent.h"

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