#pragma once

#include <GLFW/glfw3.h>

#include <array>

namespace AVulkan
{
	class AGraphicsPipeline
	{
	public:
		VkPipeline Create(VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass);
		void Dispose(VkDevice& logicalDevice, VkPipeline& graphicsPipeline);

	private:
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		VkPipelineLayout pipelineLayout;

		VkViewport* viewport;
		VkRect2D* scissor;

		VkPipelineColorBlendAttachmentState* colorBlendAttachment;

		std::array<VkPipelineShaderStageCreateInfo, 2>  CreateShadersModules(VkDevice& logicalDevice);
		void DisposeShadersModules(VkDevice& logicalDevice);

		void CreatePipelineLayout(VkDevice& logicalDevice);
		void DisposePipelineLayout(VkDevice& logicalDevice);

		void CreateViewport(VkExtent2D& swapChainExtent);
		void CreateScissor(VkExtent2D& swapChainExtent);
		void CreateColorBlendAttachment();

		VkPipelineVertexInputStateCreateInfo SetupVertexInputData();
		VkPipelineInputAssemblyStateCreateInfo SetupInputAssemblyData();
		VkPipelineViewportStateCreateInfo SetupViewportAndScissor(VkExtent2D& swapChainExtent);
		VkPipelineRasterizationStateCreateInfo SetupRasterizer();
		VkPipelineMultisampleStateCreateInfo SetupMultisampling();
		VkPipelineColorBlendStateCreateInfo SetupColorsBlending();


	};
}