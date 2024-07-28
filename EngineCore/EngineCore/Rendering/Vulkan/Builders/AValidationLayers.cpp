#include "EngineCore/Pch.h"
#include "AValidationLayers.h"

#include <GLFW/glfw3.h>

namespace AVulkan
{
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	void AValidationLayers::Setup(VkInstanceCreateInfo& createInfo)
	{
#ifdef DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif
	}

	void AValidationLayers::Setup(VkDeviceCreateInfo& createInfo)
	{
#ifdef DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif
	}
}