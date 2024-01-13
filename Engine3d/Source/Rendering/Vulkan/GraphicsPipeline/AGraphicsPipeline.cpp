#include "Pch.h"
#include "AGraphicsPipeline.h"
#include "Rendering/Vulkan/AShaderModule.h"
#include "spdlog/spdlog.h"

namespace AVulkan
{
	void AGraphicsPipeline::Create(VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass)
	{
		spdlog::info("Create graphics pipeline");

		auto shadersStages = CreateShadersModules(logicalDevice);
		CreatePipelineLayout(logicalDevice);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shadersStages.data();

		auto vertexInput = SetupVertexInputData();
		pipelineInfo.pVertexInputState = &vertexInput;

		auto inputAssembly = SetupInputAssemblyData();
		pipelineInfo.pInputAssemblyState = &inputAssembly;

		auto viewportState = SetupViewportAndScissor(swapChainExtent);
		pipelineInfo.pViewportState = &viewportState;

		auto rasterizer = SetupRasterizer();
		pipelineInfo.pRasterizationState = &rasterizer;

		auto multisample = SetupMultisampling();
		pipelineInfo.pMultisampleState = &multisample;

		auto colorBlend = SetupColorsBlending();
		pipelineInfo.pColorBlendState = &colorBlend;

		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;

		auto createState = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);

		delete colorBlendAttachment;
		delete scissor;
		delete viewport;

		if (createState != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline, state" + createState);
		}
	}

	void AGraphicsPipeline::Dispose(VkDevice& logicalDevice)
	{
		spdlog::info("Dispose graphics pipeline");

		vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
		DisposeShadersModules(logicalDevice);
		DisposePipelineLayout(logicalDevice);

	}

	std::array<VkPipelineShaderStageCreateInfo, 2> AGraphicsPipeline::CreateShadersModules(VkDevice& logicalDevice)
	{
		AShaderModule shaderModule;
		vertShaderModule = shaderModule.CreateModule("vert.spv", logicalDevice);
		fragShaderModule = shaderModule.CreateModule("frag.spv", logicalDevice);

		std::array< VkPipelineShaderStageCreateInfo, 2> shaderStages;
		shaderStages[0] = shaderModule.SetupStageInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
		shaderStages[1] = shaderModule.SetupStageInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);

		return shaderStages;
	}

	void AGraphicsPipeline::DisposeShadersModules(VkDevice& logicalDevice)
	{
		AShaderModule shaderModule;
		shaderModule.DisposeModule(logicalDevice, vertShaderModule);
		shaderModule.DisposeModule(logicalDevice, fragShaderModule);
	}

	
	VkPipelineVertexInputStateCreateInfo AGraphicsPipeline::SetupVertexInputData()
	{
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo AGraphicsPipeline::SetupInputAssemblyData()
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		return inputAssembly;
	}

	void AGraphicsPipeline::CreateViewport(VkExtent2D& swapChainExtent)
	{
		viewport = new VkViewport();
		viewport->x = 0.0f;
		viewport->y = 0.0f;
		viewport->width = (float)swapChainExtent.width;
		viewport->height = (float)swapChainExtent.height;
		viewport->minDepth = 0.0f;
		viewport->maxDepth = 1.0f;
	}

	void AGraphicsPipeline::CreateScissor(VkExtent2D& swapChainExtent)
	{
		scissor = new VkRect2D();
		scissor->offset = { 0, 0 };
		scissor->extent = swapChainExtent;
	}

	VkPipelineViewportStateCreateInfo AGraphicsPipeline::SetupViewportAndScissor(VkExtent2D& swapChainExtent)
	{
		CreateViewport(swapChainExtent);
		CreateScissor(swapChainExtent);

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;
		viewportState.pViewports = viewport;
		viewportState.pScissors = scissor;

		return viewportState;
	}

	VkPipelineRasterizationStateCreateInfo AGraphicsPipeline::SetupRasterizer()
	{
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;

		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;

		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;
		return rasterizer;
	}

	VkPipelineMultisampleStateCreateInfo AGraphicsPipeline::SetupMultisampling()
	{
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr; 
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;
		return multisampling;
	}

	void AGraphicsPipeline::CreateColorBlendAttachment()
	{
		colorBlendAttachment = new VkPipelineColorBlendAttachmentState();
		colorBlendAttachment->colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment->blendEnable = VK_TRUE;

		colorBlendAttachment->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment->colorBlendOp = VK_BLEND_OP_ADD;

		colorBlendAttachment->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment->alphaBlendOp = VK_BLEND_OP_ADD;
	}

	VkPipelineColorBlendStateCreateInfo AGraphicsPipeline::SetupColorsBlending()
	{
		CreateColorBlendAttachment();
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = colorBlendAttachment;
		return colorBlending;
	}

	void AGraphicsPipeline::CreatePipelineLayout(VkDevice& logicalDevice)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		auto createStatus = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout);
		if (createStatus != VK_SUCCESS) 
		{
			throw std::runtime_error("Failed to create pipeline layout, status: " + createStatus);
		}
	}

	void AGraphicsPipeline::DisposePipelineLayout(VkDevice& logicalDevice) 
	{
		vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	}
}