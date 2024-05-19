#pragma once

#include <GLFW/glfw3.h>

#include "Rendering/Vulkan/Extensions/BufferExtension.h"
#include "Rendering/Vulkan/Models/BufferModel.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Components/UboViewProjectionComponent.h"

namespace AVulkan
{
	class AUniformBufferVulkan
	{
	public:
		Ref<BufferModel> Create(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);
		void Dispose(VkDevice& logicalDevice, Ref<BufferModel> bufferModel);

	};
}
