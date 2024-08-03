#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <vector>

#include "EngineCore/Rendering/PipelineConfig.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/Builders/AShaderModule.h"
#include "EngineCore/Rendering/Vulkan/Builders/AVertexBuffer.h"

#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboModelComponent.h"

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