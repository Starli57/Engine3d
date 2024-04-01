#pragma once

#include <GLFW/glfw3.h>
#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/VulkanModel.h"

namespace AVulkan
{
	class ALogicalDevice
	{
	public:
		void Create(Ref<VulkanModel> vulkanModel) const;
		void Dispose(VkDevice& logicalDevice) const;
	};
}