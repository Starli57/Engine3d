#include "Pch.h"
#include "ValidationLayers.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

void ValidationLayers::SetupValidationLayers(VkInstanceCreateInfo& createInfo)
{
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
}
