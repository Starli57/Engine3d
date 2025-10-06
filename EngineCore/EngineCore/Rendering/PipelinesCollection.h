#pragma once

#include <unordered_map>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "Vulkan/VulkanPipelineConfig.h"

class PipelinesCollection
{
public:
	std::unordered_map<std::string, Ref<Engine::VulkanPipelineConfig>> pipelinesConfigs;

	PipelinesCollection(const Ref<ProjectSettings>& projectSettings);
	~PipelinesCollection();
};

