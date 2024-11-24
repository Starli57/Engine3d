#pragma once

#include <GLFW/glfw3.h>

#include <unordered_map>

#include "EngineCore/Core/AssetsDatabase.h"

#include "EngineCore/Rendering/Vulkan/MeshVulkan.h"
#include "EngineCore/Rendering/Vulkan/TextureVulkan.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/Descriptors.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/UboModelComponent.h"

namespace VkUtils
{
	void AllocateCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers, int buffersCount);
	void FreeCommandBuffers(VkDevice& logicalDevice, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers);

	void BeginCommandBuffer(VkCommandBuffer& commandBuffer);
	void EndCommandBuffer(VkCommandBuffer& commandBuffer);

	void BindPipeline(VkCommandBuffer& commandBuffer, Ref<PipelineVulkan> pipeline);
	void BindVertexAndIndexBuffers(VkCommandBuffer& commandBuffer, Ref<AVulkan::MeshVulkan> meshVulkan, Ref<AssetsDatabase> assetsDatabase);
}