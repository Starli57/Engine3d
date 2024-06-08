#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Utilities/BufferUtility.h"
#include "Rendering/Vulkan/Models/BufferModel.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Components/UboViewProjectionComponent.h"

namespace AVulkan
{
	class AUniformBufferVulkan
	{
	public:
		Ref<BufferModel> Create(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);
		void Dispose(VkDevice& logicalDevice, Ref<BufferModel> bufferModel);

	};
}
