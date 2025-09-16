#include "EngineCore/Pch.h"

#include "VkPipelineWrapper.h"

#include "VkShaderModuleWrapper.h"
#include "VertexBufferUtility.h"
#include "EngineCore/CustomAssert.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Rollback/Rollback.h"
#include "spdlog/spdlog.h"

namespace VulkanApi
{
#pragma optimize("", off)
	void VkPipelineWrapper::Create(const Ref<Engine::VulkanPipelineConfig>& pipelineConfig, VkDevice& logicalDevice,
		VkRenderPass& renderpass, VkExtent2D& swapChainExtent, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkSampleCountFlagBits msaa,
		Ref<PipelineVulkan>& pipeline)
	{
		spdlog::info("Create graphics pipeline");
		try
		{
			initializationRollback = CreateUniqueRef<Engine::Rollback>("VkPipelineInit");

			auto vertModule = CreateShaderModule(pipelineConfig->vertShaderPath, logicalDevice);
			auto fragModule = CreateShaderModule(pipelineConfig->fragShaderPath, logicalDevice);

			std::array< VkPipelineShaderStageCreateInfo, 2> shaderStages;
			shaderStages[0] = SetupShaderStageInfo(vertModule, VK_SHADER_STAGE_VERTEX_BIT);
			shaderStages[1] = SetupShaderStageInfo(fragModule, VK_SHADER_STAGE_FRAGMENT_BIT);

			VkPushConstantRange pushConstantRange;
			pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			pushConstantRange.offset = 0;
			pushConstantRange.size = sizeof(UboModelComponent);

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
			pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
			pipelineLayoutInfo.pushConstantRangeCount = 1;
			pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

			auto createStatus = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipeline->layout);
			//---> Pipeline layout created

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			if (pipelineConfig->useVertices)
			{
				auto bindingDescription = GetVertexInputBindingDescription();
				auto attributeDescriptions = GetVertexInputAttributeDescriptions();
				vertexInputInfo.vertexBindingDescriptionCount = 1;
				vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
				vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
				vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
			}
			
			auto inputAssembly = SetupInputAssemblyData();
			auto viewportState = SetupViewportAndScissor(swapChainExtent);
			auto rasterizer = SetupRasterizer(pipelineConfig);
			auto multisampling = SetupMultisampling(msaa);
			auto colorBlend = SetupColorsBlending(pipelineConfig);

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
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlend;
			pipelineInfo.pDynamicState = nullptr;
			pipelineInfo.layout = pipeline->layout;
			pipelineInfo.renderPass = renderpass;
			pipelineInfo.subpass = 0;
			pipelineInfo.pDepthStencilState = &depthStencil;

			auto createState = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline->pipeline);
			Engine::CAssert::Check(createState == VK_SUCCESS, "Failed to create graphics pipeline, state" + createState);

			initializationRollback->Dispose();

			DisposeShaderModule(logicalDevice, fragModule);
			DisposeShaderModule(logicalDevice, vertModule);
		}
		catch (const std::exception& e)
		{
			initializationRollback->Dispose();
			throw;
		}
	}
#pragma optimize("", on)

	void VkPipelineWrapper::ReCreate(Ref<PipelineVulkan>& pipeline, const Ref<Engine::VulkanPipelineConfig>& pipelineConfig, VkDevice& logicalDevice,
		VkRenderPass& renderpass, VkExtent2D& swapChainExtent, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, VkSampleCountFlagBits msaa)
	{
		Dispose(pipeline, logicalDevice);
		Create(pipelineConfig, logicalDevice, renderpass, swapChainExtent, descriptorSetLayouts, msaa, pipeline);
	}

	void VkPipelineWrapper::Dispose(const Ref<PipelineVulkan>& pipeline, const VkDevice& logicalDevice) const
	{
		vkDestroyPipelineLayout(logicalDevice, pipeline->layout, nullptr);
		vkDestroyPipeline(logicalDevice, pipeline->pipeline, nullptr);
	}
	
	VkPipelineInputAssemblyStateCreateInfo VkPipelineWrapper::SetupInputAssemblyData() const
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		return inputAssembly;
	}

	VkPipelineViewportStateCreateInfo VkPipelineWrapper::SetupViewportAndScissor(const VkExtent2D& swapChainExtent) const
	{
		auto viewport = new VkViewport();
		viewport->x = 0.0f;
		viewport->y = 0.0f;
		viewport->width = static_cast<float>(swapChainExtent.width);
		viewport->height = static_cast<float>(swapChainExtent.height);
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

	VkPipelineRasterizationStateCreateInfo VkPipelineWrapper::SetupRasterizer(const Ref<Engine::VulkanPipelineConfig>& pipelineConfig) const
	{
		spdlog::info("Setup rasterizer");
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;

		rasterizer.polygonMode = pipelineConfig->polygonMode;
		rasterizer.lineWidth = 1.0f;

		rasterizer.cullMode = pipelineConfig->cullMode;
		rasterizer.frontFace = pipelineConfig->frontFace;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		return rasterizer;
	}

	VkPipelineMultisampleStateCreateInfo VkPipelineWrapper::SetupMultisampling(const VkSampleCountFlagBits msaa) const
	{
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = msaa;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr; 
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;
		return multisampling;
	}

	VkPipelineColorBlendStateCreateInfo VkPipelineWrapper::SetupColorsBlending(const Ref<Engine::VulkanPipelineConfig>& pipelineConfig) const
	{
		auto colorBlendAttachment = new VkPipelineColorBlendAttachmentState();
		colorBlendAttachment->colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment->blendEnable = pipelineConfig->transparent ? VK_TRUE : VK_FALSE;

		colorBlendAttachment->srcColorBlendFactor = pipelineConfig->transparent ? VK_BLEND_FACTOR_SRC_ALPHA : VK_BLEND_FACTOR_ONE;
		colorBlendAttachment->dstColorBlendFactor = pipelineConfig->transparent ? VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA : VK_BLEND_FACTOR_ZERO;
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