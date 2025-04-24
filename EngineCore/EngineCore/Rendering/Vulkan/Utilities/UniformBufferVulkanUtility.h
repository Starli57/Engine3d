#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboWorldComponent.h"

namespace VkUtils
{
	Ref<AVulkan::BufferModel> CreateUniformBuffer(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, 
			VkDeviceSize bufferSize);
}
