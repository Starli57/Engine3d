#pragma once

#include <GLFW/glfw3.h>
#include <vector>

namespace VulkanApi
{
	void CreateInstance(VkInstance& instance);
	void DisposeInstance(const VkInstance& instance);

	std::vector<const char*> GetGLFWRequiredExtensions();
}
