#include "EngineCore/Pch.h"
#include "PipelinesCollection.h"

PipelinesCollection::PipelinesCollection(Ref<ProjectSettigns> projectSettings)
{
	auto albedoOpaque = CreateRef<VulkanPipelineConfig>();
	albedoOpaque->pipelineName = "albedoOpaque";
	albedoOpaque->fragShaderPath = projectSettings->projectPath + "Shaders/AlbedoFrag.spv";
	albedoOpaque->vertShaderPath = projectSettings->projectPath + "Shaders/AlbedoVert.spv";
	albedoOpaque->polygonMode = VK_POLYGON_MODE_FILL;
	albedoOpaque->cullMode = VK_CULL_MODE_BACK_BIT;
	albedoOpaque->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	pipelinesConfigs.emplace(albedoOpaque->pipelineName, albedoOpaque);

	auto edges = CreateRef<VulkanPipelineConfig>();
	edges->pipelineName = "edges";
	edges->fragShaderPath = projectSettings->projectPath + "Shaders/AlbedoFrag.spv";
	edges->vertShaderPath = projectSettings->projectPath + "Shaders/AlbedoVert.spv";
	edges->polygonMode = VK_POLYGON_MODE_LINE;
	edges->cullMode = VK_CULL_MODE_BACK_BIT;
	edges->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	pipelinesConfigs.emplace(edges->pipelineName, edges);
}

PipelinesCollection::~PipelinesCollection()
{
	pipelinesConfigs.clear();
}
