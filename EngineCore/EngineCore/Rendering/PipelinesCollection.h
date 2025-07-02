#pragma once

#include <unordered_map>

#include "EngineCore/Rendering/Vulkan/Configs/VulkanPipelineConfig.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

using namespace EngineCore;
class PipelinesCollection
{
public:
	std::unordered_map<std::string, Ref<VulkanPipelineConfig>> pipelinesConfigs;

	PipelinesCollection(const Ref<ProjectSettings>& projectSettings);
	~PipelinesCollection();
};

