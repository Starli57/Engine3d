#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Rendering/PipelineConfig.h"

namespace EngineCore
{
	struct VulkanPipelineConfig : public PipelineConfig
	{
		VkPolygonMode polygonMode;
		VkCullModeFlags cullMode;
		VkFrontFace frontFace;
		bool useVertices;
	};
}