#pragma once

#include <imgui.h>
#include <vector>

#include "EngineCore/Engine.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Editor/IWindow.h"

class VulkanDebugInfo : public IWindow
{
public:
	VulkanDebugInfo(Ref<Engine> engine, AVulkan::GraphicsApiVulkan& vulkanApi);
	~VulkanDebugInfo();

	void Update() override;

private:
	Ref<Engine> engine;
	AVulkan::GraphicsApiVulkan& vulkanApi;

	std::vector<float>* fpsFrames;
	int currentFpsIndex = 0;
};