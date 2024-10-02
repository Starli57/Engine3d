#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <vector>

#include "EngineCore/Rendering/Vulkan/Configs/VulkanPipelineConfig.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/Builders/AShaderModule.h"
#include "EngineCore/Rendering/Vulkan/Utilities/VertexBufferUtility.h"

#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboModelComponent.h"

namespace AVulkan
{
	class GraphicsPipelineUtility
	{
	public:
		Ref<PipelineVulkan> Create(Ref<VulkanPipelineConfig> pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout,
			VkSampleCountFlagBits msaa);

		Ref<PipelineVulkan> ReCreate(Ref<PipelineVulkan> pipeline, Ref<VulkanPipelineConfig> pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout, VkSampleCountFlagBits msaa);

		void Dispose(Ref<PipelineVulkan> pipeline, VkDevice& logicalDevice);

	private:

		URef<Rollback> initializationRollback;

		VkPipelineInputAssemblyStateCreateInfo SetupInputAssemblyData();
		VkPipelineViewportStateCreateInfo SetupViewportAndScissor(VkExtent2D& swapChainExtent);
		VkPipelineRasterizationStateCreateInfo SetupRasterizer(Ref<VulkanPipelineConfig> pipelineConfig);
		VkPipelineMultisampleStateCreateInfo SetupMultisampling(VkSampleCountFlagBits msaa);
		VkPipelineColorBlendStateCreateInfo SetupColorsBlending();


	};
}