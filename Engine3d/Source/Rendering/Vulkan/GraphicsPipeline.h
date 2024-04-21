#pragma once

#include <GLFW/glfw3.h>

#include <array>

#include "ProjectSettings.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"
#include "SharedLib/Components/UboModelComponent.h"

namespace AVulkan
{
	class GraphicsPipeline
	{
	public:
		VkPipeline GetPipeline();
		VkPipelineLayout GetLayout();
		VkViewport* GetViewport();
		VkRect2D* GetScissor();

		GraphicsPipeline(Ref<ProjectSettigns> projectSettings, VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, Ref<Rollback> vulkanRollback);
		~GraphicsPipeline();

		void Create(VkDescriptorSetLayout& descriptorSetLayout);
		void Dispose();


	private:
		Ref<ProjectSettigns> projectSettings;

		URef<Rollback> rollback;
		URef<Rollback> initializationRollback;

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

		VkPushConstantRange pushContantRange;

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