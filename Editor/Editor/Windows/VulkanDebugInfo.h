#pragma once

#include <imgui.h>

#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/IWindow.h"

class VulkanDebugInfo : public IWindow
{
public:
	VulkanDebugInfo(AVulkan::GraphicsApiVulkan& vulkanApi);
	void Update() override;

private:
	AVulkan::GraphicsApiVulkan& vulkanApi;
};