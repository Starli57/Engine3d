#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <vector>

#include "Rendering/PipelineConfig.h"
#include "Rendering/Vulkan/PipelineVulkan.h"
#include "Rendering/Vulkan/Builders/AShaderModule.h"
#include "Rendering/Vulkan/Builders/AVertexBuffer.h"

#include "EngineShared/ProjectSettings.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"
#include "EngineShared/Components/UboModelComponent.h"

namespace AVulkan
{
	class GraphicsPipelineUtility
	{
	public:
		Ref<PipelineVulkan> Create(Ref<PipelineConfig> pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout);

		Ref<PipelineVulkan> ReCreate(Ref<PipelineVulkan> pipeline, Ref<PipelineConfig> pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout);

		void Dispose(Ref<PipelineVulkan> pipeline, VkDevice& logicalDevice);

	private:

		URef<Rollback> initializationRollback;

		VkPipelineInputAssemblyStateCreateInfo SetupInputAssemblyData();
		VkPipelineViewportStateCreateInfo SetupViewportAndScissor(VkExtent2D& swapChainExtent);
		VkPipelineRasterizationStateCreateInfo SetupRasterizer();
		VkPipelineMultisampleStateCreateInfo SetupMultisampling();
		VkPipelineColorBlendStateCreateInfo SetupColorsBlending();


	};
}