#pragma once

#include <imgui.h>

#include "Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineShared/IWindow.h"

class VulkanDebugInfo : public IWindow
{
public:
	VulkanDebugInfo(AVulkan::GraphicsApiVulkan& vulkanApi);
	void Update() override;

private:
	AVulkan::GraphicsApiVulkan& vulkanApi;
};