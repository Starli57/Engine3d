#pragma once

#include <GLFW/glfw3.h>

#include <array>

#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Data/Vertex.h"

namespace AVulkan
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass);
		~GraphicsPipeline();

		void Create(VkDescriptorSetLayout& descriptorSetLayout);
		void Dispose();

		VkPipeline GetPipeline();
		VkPipelineLayout GetLayout();

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