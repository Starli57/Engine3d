#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <vector>

#include "EngineCore/Rendering/Vulkan/Configs/VulkanPipelineConfig.h"
#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ShaderModuleUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/VertexBufferUtility.h"

#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboModelComponent.h"

using namespace EngineCore;

namespace AVulkan
{
	class GraphicsPipelineUtility
	{
	public:
		Ref<PipelineVulkan> Create(const Ref<VulkanPipelineConfig>& pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
			VkSampleCountFlagBits msaa);

		Ref<PipelineVulkan> ReCreate(const Ref<PipelineVulkan>& pipeline, const Ref<VulkanPipelineConfig>& pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkSampleCountFlagBits msaa);

		void Dispose(const Ref<PipelineVulkan>& pipeline, const VkDevice& logicalDevice) const;

	private:

		URef<Rollback> initializationRollback;

		VkPipelineInputAssemblyStateCreateInfo SetupInputAssemblyData() const;
		VkPipelineViewportStateCreateInfo SetupViewportAndScissor(const VkExtent2D& swapChainExtent) const;
		VkPipelineRasterizationStateCreateInfo SetupRasterizer(const Ref<VulkanPipelineConfig>& pipelineConfig) const;
		VkPipelineMultisampleStateCreateInfo SetupMultisampling(VkSampleCountFlagBits msaa) const;
		VkPipelineColorBlendStateCreateInfo SetupColorsBlending(const Ref<VulkanPipelineConfig>& pipelineConfig) const;


	};
}