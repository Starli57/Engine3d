#include "Pch.h"

#include "Buffers/AVertexBuffer.h"

#include "AGraphicsPipeline.h"
#include "AShaderModule.h"

#include "spdlog/spdlog.h"

namespace AVulkan
{
	AGraphicsPipeline::AGraphicsPipeline(VkDevice& logicalDevice, VkExtent2D& swapChainExtent, VkRenderPass& renderPass) 
	{
		rollback = new Rollback();

		this->logicalDevice = logicalDevice;
		this->swapChainExtent = swapChainExtent;
		this->renderPass = renderPass;
	}

	AGraphicsPipeline::~AGraphicsPipeline()
	{
		rollback->Dispose();
		delete rollback;
	}

	VkPipeline AGraphicsPipeline::Create()
	{
		spdlog::info("Create graphics pipeline");

		try
		{
			auto shadersStages = CreateShadersModules();
			CreatePipelineLayout();

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

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shadersStages.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisample;
			pipelineInfo.pColorBlendState = &colorBlend;
			pipelineInfo.pDynamicState = nullptr;
			pipelineInfo.pDepthStencilState = nullptr;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = renderPass;
			pipelineInfo.subpass = 0;

			VkPipeline graphicsPipeline;
			auto createState = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);

			if (createState != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create graphics pipeline, state" + createState);
			}

			rollback->Dispose();
			spdlog::info("Graphics pipeline successfully created");

			return graphicsPipeline;
		}
		catch (const std::exception& e)
		{
			rollback->Dispose();
			throw e;
		}
	}

	void AGraphicsPipeline::Dispose(VkPipeline& graphicsPipeline)
	{
		spdlog::info("Dispose graphics pipeline");
		vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	}

	std::array<VkPipelineShaderStageCreateInfo, 2> AGraphicsPipeline::CreateShadersModules()
	{
		AShaderModule shaderModule;
		vertShaderModule = shaderModule.CreateModule("vert.spv", logicalDevice);
		fragShaderModule = shaderModule.CreateModule("frag.spv", logicalDevice);

		std::array< VkPipelineShaderStageCreateInfo, 2> shaderStages;
		shaderStages[0] = shaderModule.SetupStageInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
		shaderStages[1] = shaderModule.SetupStageInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);

		rollback->Add([this]
		{
			spdlog::info("dispose ShadersModules");
			AShaderModule shaderModule;
			shaderModule.DisposeModule(logicalDevice, vertShaderModule);
			shaderModule.DisposeModule(logicalDevice, fragShaderModule);
		});

		return shaderStages;
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

		rollback->Add([this] { delete viewport; });
	}

	void AGraphicsPipeline::CreateScissor(VkExtent2D& swapChainExtent)
	{
		scissor = new VkRect2D();
		scissor->offset = { 0, 0 };
		scissor->extent = swapChainExtent;

		rollback->Add([this] { delete scissor; });
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

		rollback->Add([this] { delete colorBlendAttachment; });
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

	void AGraphicsPipeline::CreatePipelineLayout()
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

		rollback->Add([this]
		{
			spdlog::info("dispose pipeline layout");
			vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
		});
	
	}
}