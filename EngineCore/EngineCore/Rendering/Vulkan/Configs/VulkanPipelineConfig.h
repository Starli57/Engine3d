#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/PipelineConfig.h"

struct VulkanPipelineConfig : public PipelineConfig
{
	VkPolygonMode polygonMode;
};