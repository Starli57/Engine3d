#pragma once

#include <GLFW/glfw3.h>

#include <array>

#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"

namespace AVulkan
{
	class GraphicsPipeline
	{
	public:
		VkPipeline GetPipeline();
		VkPipelineLayout GetLayout();

		GraphicsPipeline(VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, Rollback* vulkanRollback);
		~GraphicsPipeline();

		void Create(VkDescriptorSetLayout& descriptorSetLayout);
		void Dispose();


	private:
		Rollback* rollback;
		Rollback* initializationRollback;

		VkDevice logicalDevice;
		VkExtent2D swapChainExtent;
		VkRenderPass renderPass;

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;

		VkViewport* viewport;
		VkRect2D* scissor;

		VkPipelineColorBlendAttachmentState* colorBlendAttachment;

		std::array<VkPipelineShaderStageCreateInfo, 2>  CreateShadersModules();
		void CreatePipelineLayout(VkDescriptorSetLayout& descriptorSetLayout);
		void CreateViewport(VkExtent2D& swapChainExtent);
		void CreateScissor(VkExtent2D& swapChainExtent);
		void CreateColorBlendAttachment();

		VkPipelineInputAssemblyStateCreateInfo SetupInputAssemblyData();
		VkPipelineViewportStateCreateInfo SetupViewportAndScissor(VkExtent2D& swapChainExtent);
		VkPipelineRasterizationStateCreateInfo SetupRasterizer();
		VkPipelineMultisampleStateCreateInfo SetupMultisampling();
		VkPipelineColorBlendStateCreateInfo SetupColorsBlending();


	};
}