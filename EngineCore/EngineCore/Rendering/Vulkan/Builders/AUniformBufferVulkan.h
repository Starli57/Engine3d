#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Rendering/Vulkan/Utilities/BufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"

namespace AVulkan
{
	class AUniformBufferVulkan
	{
	public:
		Ref<BufferModel> Create(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkDeviceSize bufferSize, Ref<Rollback> rollback);
	};
}
