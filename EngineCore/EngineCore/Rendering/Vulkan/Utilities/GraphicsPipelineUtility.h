#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Rendering/Vulkan/PipelineVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanPipelineConfig.h"

namespace Engine
{
	class Rollback;
}

namespace AVulkan
{
	class GraphicsPipelineUtility
	{
	public:
		Ref<PipelineVulkan> Create(const Ref<Engine::VulkanPipelineConfig>& pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
			VkSampleCountFlagBits msaa);

		Ref<PipelineVulkan> ReCreate(const Ref<PipelineVulkan>& pipeline, const Ref<Engine::VulkanPipelineConfig>& pipelineConfig, VkDevice& logicalDevice,
			VkRenderPass& renderpass, VkExtent2D& swapChainExtent, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkSampleCountFlagBits msaa);

		void Dispose(const Ref<PipelineVulkan>& pipeline, const VkDevice& logicalDevice) const;

	private:

		URef<Engine::Rollback> initializationRollback;

		VkPipelineInputAssemblyStateCreateInfo SetupInputAssemblyData() const;
		VkPipelineViewportStateCreateInfo SetupViewportAndScissor(const VkExtent2D& swapChainExtent) const;
		VkPipelineRasterizationStateCreateInfo SetupRasterizer(const Ref<Engine::VulkanPipelineConfig>& pipelineConfig) const;
		VkPipelineMultisampleStateCreateInfo SetupMultisampling(VkSampleCountFlagBits msaa) const;
		VkPipelineColorBlendStateCreateInfo SetupColorsBlending(const Ref<Engine::VulkanPipelineConfig>& pipelineConfig) const;


	};
}
