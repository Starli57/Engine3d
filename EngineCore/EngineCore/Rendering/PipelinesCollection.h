#pragma once

#include <unordered_map>

#include "PipelineConfig.h"
#include "EngineCore/Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

class PipelinesCollection
{
public:
	std::unordered_map<std::string, Ref<PipelineConfig>> pipelinesConfigs;

	PipelinesCollection(Ref<ProjectSettigns> projectSettings);
	~PipelinesCollection();
};

