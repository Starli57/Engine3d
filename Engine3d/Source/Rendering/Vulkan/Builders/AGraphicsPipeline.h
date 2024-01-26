#pragma once

#include <GLFW/glfw3.h>

#include <array>

#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Data/Vertex.h"

namespace AVulkan
{
	class AGraphicsPipeline
	{
	public:
		AGraphicsPipeline(VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass);
		~AGraphicsPipeline();

		VkPipeline Create();
		void Dispose(VkPipeline& graphicsPipeline);

	private:
		Rollback* rollback;

		VkDevice logicalDevice;
		VkExtent2D swapChainExtent;
		VkRenderPass renderPass;

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		VkPipelineLayout pipelineLayout;

		VkViewport* viewport;
		VkRect2D* scissor;

		VkPipelineColorBlendAttachmentState* colorBlendAttachment;

		std::array<VkPipelineShaderStageCreateInfo, 2>  CreateShadersModules();
		void CreatePipelineLayout();
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