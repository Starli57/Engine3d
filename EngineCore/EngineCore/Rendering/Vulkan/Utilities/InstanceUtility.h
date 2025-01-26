#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Rendering/Vulkan/Utilities/ValidationLayersUtility.h"

namespace VkUtils
{
	void CreateInstance(VkInstance& instance);
	void DisposeInstance(const VkInstance& instance);

	std::vector<const char*> GetGLFWRequiredExtensions();
}
