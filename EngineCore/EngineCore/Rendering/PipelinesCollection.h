#pragma once

#include <unordered_map>

#include "EngineCore/Rendering/Vulkan/Configs/VulkanPipelineConfig.h"
#include "EngineCore/Rendering/Vulkan/Utilities/GraphicsPipelineUtility.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

class PipelinesCollection
{
public:
	std::unordered_map<std::string, Ref<VulkanPipelineConfig>> pipelinesConfigs;

	PipelinesCollection(const Ref<ProjectSettings>& projectSettings);
	~PipelinesCollection();
};

