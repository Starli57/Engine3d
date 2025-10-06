#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	void CreateSurface(VulkanContext* vulkanContext, GLFWwindow* window);
	void DisposeSurface(VulkanContext* vulkanContext);
}
