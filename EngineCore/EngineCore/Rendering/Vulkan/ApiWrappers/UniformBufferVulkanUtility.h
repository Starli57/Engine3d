#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"
#include "EngineCore/Core/Ref.h"

namespace VulkanApi
{
	Ref<BufferModel> CreateUniformBuffer(const VulkanContext* vulkanContext, VkDeviceSize bufferSize);
}
