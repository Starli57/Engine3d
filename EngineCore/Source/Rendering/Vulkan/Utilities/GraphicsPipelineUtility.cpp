#include "Pch.h"

#include "GraphicsPipelineUtility.h"

#include "spdlog/spdlog.h"

namespace AVulkan
{
	Ref<VulkanPipeline> GraphicsPipelineUtility::Create(Ref<PipelineConfig> pipelineConfig, VkDevice& logicalDevice, 
		VkRenderPass& renderpass, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout)
	{
		spdlog::info("Create graphics pipeline");
		auto pipeline = CreateRef<VulkanPipeline>();

		try
		{
			initializationRollback = CreateUniqueRef<Rollback>("VkPipelineInit");

			AShaderModule shaderModuleUtility;
			auto vertModule = shaderModuleUtility.CreateModule(pipelineConfig->vertShaderPath, logicalDevice);
			auto fragModule = shaderModuleUtility.CreateModule(pipelineConfig->fragShaderPath, logicalDevice);

			std::array< VkPipelineShaderStageCreateInfo, 2> shaderStages;
			shaderStages[0] = shaderModuleUtility.SetupStageInfo(vertModule, VK_SHADER_STAGE_VERTEX_BIT);
			shaderStages[1] = shaderModuleUtility.SetupStageInfo(fragModule, VK_SHADER_STAGE_FRAGMENT_BIT);

			VkPushConstantRange pushConstantRange;
			pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			pushConstantRange.offset = 0;
			pushConstantRange.size = sizeof(UboModelComponent);

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
			pipelineLayoutInfo.pushConstantRangeCount = 1;
			pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

			auto createStatus = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipeline->layout);
			//---> Pipeline layout created

			auto bindingDescription = AVertexBuffer().GetBindingDescription();
			auto attributeDescriptions = AVertexBuffer().GetAttributeDescriptions();

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

			auto inputAssembly = SetupInputAssemblyData();
			auto viewportState = SetupViewportAndScissor(swapChainExtent);
			auto rasterizer = SetupRasterizer();
			auto multisample = SetupMultisampling();
			auto colorBlend = SetupColorsBlending();

			VkPipelineDepthStencilStateCreateInfo depthStencil{};
			depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencil.depthTestEnable = VK_TRUE;
			depthStencil.depthWriteEnable = VK_TRUE;
			depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
			depthStencil.depthBoundsTestEnable = VK_FALSE;
			depthStencil.stencilTestEnable = VK_FALSE;

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisample;
			pipelineInfo.pColorBlendState = &colorBlend;
			pipelineInfo.pDynamicState = nullptr;
			pipelineInfo.layout = pipeline->layout;
			pipelineInfo.renderPass = renderpass;
			pipelineInfo.subpass = 0;
			pipelineInfo.pDepthStencilState = &depthStencil;

			auto createState = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline->pipeline);
			CAssert::Check(createState == VK_SUCCESS, "Failed to create graphics pipeline, state" + createState);
			//---> Pipeline created

			initializationRollback->Dispose();

			shaderModuleUtility.DisposeModule(logicalDevice, fragModule);
			shaderModuleUtility.DisposeModule(logicalDevice, vertModule);
		}
		catch (const std::exception& e)
		{
			initializationRollback->Dispose();
			throw e;
		}
		return pipeline;
	}

	Ref<VulkanPipeline> GraphicsPipelineUtility::ReCreate(Ref<VulkanPipeline> pipeline, Ref<PipelineConfig> pipelineConfig, VkDevice& logicalDevice,
		VkRenderPass& renderpass, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout)
	{
		Dispose(pipeline, logicalDevice);
		return Create(pipelineConfig, logicalDevice, renderpass, swapChainExtent, descriptorSetLayout);
	}

	void GraphicsPipelineUtility::Dispose(Ref<VulkanPipeline> pipeline, VkDevice& logicalDevice)
	{
		vkDestroyPipelineLayout(logicalDevice, pipeline->layout, nullptr);
		vkDestroyPipeline(logicalDevice, pipeline->pipeline, nullptr);
	}
	
	VkPipelineInputAssemblyStateCreateInfo GraphicsPipelineUtility::SetupInputAssemblyData()
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		return inputAssembly;
	}

	VkPipelineViewportStateCreateInfo GraphicsPipelineUtility::SetupViewportAndScissor(VkExtent2D& swapChainExtent)
	{
		auto viewport = new VkViewport();
		viewport->x = 0.0f;
		viewport->y = 0.0f;
		viewport->width = (float)swapChainExtent.width;
		viewport->height = (float)swapChainExtent.height;
		viewport->minDepth = 0.0f;
		viewport->maxDepth = 1.0f;

		auto scissor = new VkRect2D();
		scissor->offset = { 0, 0 };
		scissor->extent = swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;
		viewportState.pViewports = viewport;
		viewportState.pScissors = scissor;

		initializationRollback->Add([this, viewport, scissor]
		{
			delete scissor;
			delete viewport;
		});

		return viewportState;
	}

	VkPipelineRasterizationStateCreateInfo GraphicsPipelineUtility::SetupRasterizer()
	{
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;

		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;

		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		return rasterizer;
	}

	VkPipelineMultisampleStateCreateInfo GraphicsPipelineUtility::SetupMultisampling()
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

	VkPipelineColorBlendStateCreateInfo GraphicsPipelineUtility::SetupColorsBlending()
	{
		auto colorBlendAttachment = new VkPipelineColorBlendAttachmentState();
		colorBlendAttachment->colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment->blendEnable = VK_TRUE;

		colorBlendAttachment->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment->colorBlendOp = VK_BLEND_OP_ADD;

		colorBlendAttachment->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment->alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = colorBlendAttachment;

		initializationRollback->Add([this, colorBlendAttachment]
		{
			delete colorBlendAttachment;
		});
	
		return colorBlending;
	}
}