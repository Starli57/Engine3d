#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace VulkanApi
{
	void CreateSurface(const Ref<VulkanContext>& vulkanContext);
	void DisposeSurface(const Ref<VulkanContext>& vulkanContext);
}
