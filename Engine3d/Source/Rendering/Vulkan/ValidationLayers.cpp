#include "Pch.h"
#include "ValidationLayers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

void ValidationLayers::Setup(VkInstanceCreateInfo& createInfo)
{
#ifdef DEBUG
	//todo: check validation layers support before enabling
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#endif
}

void ValidationLayers::Setup(VkDeviceCreateInfo& createInfo)
{
#ifdef DEBUG
	//todo: check validation layers support before enabling
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#endif
}
