#pragma once

#include <unordered_map>

#include "PipelineConfig.h"
#include "Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"
#include "EngineShared/Ref.h"
#include "EngineShared/ProjectSettings.h"

class PipelinesCollection
{
public:
	std::unordered_map<std::string, Ref<PipelineConfig>> pipelinesConfigs;

	PipelinesCollection(Ref<ProjectSettigns> projectSettings);
	~PipelinesCollection();
};

