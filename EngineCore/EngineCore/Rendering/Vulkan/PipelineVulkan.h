#pragma once

#include <GLFW/glfw3.h>

struct PipelineVulkan
{
	VkPipeline pipeline = VK_NULL_HANDLE;
	VkPipelineLayout layout;
};

