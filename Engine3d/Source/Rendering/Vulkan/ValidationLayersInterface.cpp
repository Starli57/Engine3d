#include "Pch.h"
#include "ValidationLayersInterface.h"

#include <GLFW/glfw3.h>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

void ValidationLayersInterface::Setup(VkInstanceCreateInfo& createInfo)
{
#ifdef DEBUG
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#else
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;
#endif
}

void ValidationLayersInterface::Setup(VkDeviceCreateInfo& createInfo)
{
#ifdef DEBUG
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#else
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;
#endif
}
