#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Core/Ref.h"

namespace VkUtils
{
	void AllocateCommandBuffers(const VkDevice& logicalDevice, const VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount);
	void FreeCommandBuffers(const VkDevice& logicalDevice, const VkCommandPool& commandPool, const std::vector<VkCommandBuffer>& commandBuffers);

	void BeginCommandBuffer(const VkCommandBuffer& commandBuffer);
	void EndCommandBuffer(const VkCommandBuffer& commandBuffer);

	void BindPipeline(const VkCommandBuffer& commandBuffer, const Ref<PipelineVulkan>& pipeline);
	void BindVertexAndIndexBuffers(const VkCommandBuffer& commandBuffer, int32_t meshVulkan, const Ref<Engine::ResourcesStorageVulkan>& assetsDatabase);
}