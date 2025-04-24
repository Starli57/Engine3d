#include "EngineCore/Pch.h"
#include "PipelinesCollection.h"

PipelinesCollection::PipelinesCollection(const Ref<ProjectSettings>& projectSettings)
{
	auto shadowPass = CreateRef<VulkanPipelineConfig>();
	shadowPass->pipelineName = "shadowPass";
	shadowPass->fragShaderPath = projectSettings->projectPath + "Shaders/ShadowPassFrag.spv";
	shadowPass->vertShaderPath = projectSettings->projectPath + "Shaders/ShadowPassVert.spv";
	shadowPass->polygonMode = VK_POLYGON_MODE_FILL;
	shadowPass->cullMode = VK_CULL_MODE_BACK_BIT;
	shadowPass->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	shadowPass->transparent = false;
	pipelinesConfigs.emplace(shadowPass->pipelineName, shadowPass);
	
	auto opaque = CreateRef<VulkanPipelineConfig>();
	opaque->pipelineName = "opaque";
	opaque->fragShaderPath = projectSettings->projectPath + "Shaders/OpaqueFrag.spv";
	opaque->vertShaderPath = projectSettings->projectPath + "Shaders/OpaqueVert.spv";
	opaque->polygonMode = VK_POLYGON_MODE_FILL;
	opaque->cullMode = VK_CULL_MODE_BACK_BIT;
	opaque->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	opaque->transparent = false;
	pipelinesConfigs.emplace(opaque->pipelineName, opaque);

	auto transparent = CreateRef<VulkanPipelineConfig>();
	transparent->pipelineName = "transparent";
	transparent->fragShaderPath = projectSettings->projectPath + "Shaders/OpaqueFrag.spv";
	transparent->vertShaderPath = projectSettings->projectPath + "Shaders/OpaqueVert.spv";
	transparent->polygonMode = VK_POLYGON_MODE_FILL;
	transparent->cullMode = VK_CULL_MODE_BACK_BIT;
	transparent->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	transparent->transparent = true;
	pipelinesConfigs.emplace(transparent->pipelineName, transparent);
	
	auto edges = CreateRef<VulkanPipelineConfig>();
	edges->pipelineName = "edges";
	edges->fragShaderPath = projectSettings->projectPath + "Shaders/OpaqueFrag.spv";
	edges->vertShaderPath = projectSettings->projectPath + "Shaders/OpaqueVert.spv";
	edges->polygonMode = VK_POLYGON_MODE_LINE;
	edges->cullMode = VK_CULL_MODE_BACK_BIT;
	edges->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	edges->transparent = false;
	pipelinesConfigs.emplace(edges->pipelineName, edges);
	
	// auto grid = CreateRef<VulkanPipelineConfig>();
	// grid->pipelineName = "grid";
	// grid->fragShaderPath = projectSettings->projectPath + "Shaders/GridFrag.spv";
	// grid->vertShaderPath = projectSettings->projectPath + "Shaders/GridVert.spv";
	// grid->polygonMode = VK_POLYGON_MODE_LINE;
	// grid->cullMode = VK_CULL_MODE_BACK_BIT;
	// grid->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	// grid->transparent = false;
	// pipelinesConfigs.emplace(grid->pipelineName, grid);
}

PipelinesCollection::~PipelinesCollection()
{
	pipelinesConfigs.clear();
}
