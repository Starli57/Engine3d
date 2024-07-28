#include "EngineCore/Pch.h"
#include "PipelinesCollection.h"

PipelinesCollection::PipelinesCollection(Ref<ProjectSettigns> projectSettings)
{
	auto albedoOpaque = CreateRef<PipelineConfig>();
	albedoOpaque->pipelineName = "albedoOpaque";
	albedoOpaque->fragShaderPath = projectSettings->projectPath + "Shaders/AlbedoFrag.spv";
	albedoOpaque->vertShaderPath = projectSettings->projectPath + "Shaders/AlbedoVert.spv";
	pipelinesConfigs.emplace(albedoOpaque->pipelineName, albedoOpaque);

	auto vertexColorOpaque = CreateRef<PipelineConfig>();
	vertexColorOpaque->pipelineName = "vertexColorOpaque";
	vertexColorOpaque->fragShaderPath = projectSettings->projectPath + "Shaders/ColorOnlyFrag.spv";
	vertexColorOpaque->vertShaderPath = projectSettings->projectPath + "Shaders/AlbedoVert.spv";
	pipelinesConfigs.emplace(vertexColorOpaque->pipelineName, vertexColorOpaque);
}

PipelinesCollection::~PipelinesCollection()
{
	pipelinesConfigs.clear();
}
