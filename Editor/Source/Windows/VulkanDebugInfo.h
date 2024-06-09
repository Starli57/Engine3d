#pragma once

#include <imgui.h>

#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "EngineShared/IWindow.h"

class VulkanDebugInfo : public IWindow
{
public:
	VulkanDebugInfo(AVulkan::VulkanGraphicsApi& vulkanApi);
	void Update() override;

private:
	AVulkan::VulkanGraphicsApi& vulkanApi;
};