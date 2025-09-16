#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Rendering/Vulkan/ApiWrappers/ValidationLayersUtility.h"

namespace VulkanApi
{
	void CreateInstance(VkInstance& instance);
	void DisposeInstance(const VkInstance& instance);

	std::vector<const char*> GetGLFWRequiredExtensions();
}
