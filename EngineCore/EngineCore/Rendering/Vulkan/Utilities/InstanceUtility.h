#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Rendering/Vulkan/Builders/AValidationLayers.h"

namespace VkUtils
{
	void CreateInstance(VkInstance& instance);
	void DisposeInstance(VkInstance& instance);

	std::vector<const char*> GetGLFWRequiredExtensions();
}
