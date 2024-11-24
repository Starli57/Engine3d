#include "EngineCore/Pch.h"
#include "ValidationLayersUtility.h"

#include <GLFW/glfw3.h>

namespace VkUtils
{
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	void SetupValidationLayers(VkInstanceCreateInfo& createInfo)
	{
#ifdef DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif
	}

	void SetupValidationLayers(VkDeviceCreateInfo& createInfo)
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