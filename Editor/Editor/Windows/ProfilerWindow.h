#pragma once

#include <imgui.h>
#include <vector>

#include "EngineCore/Engine.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Profiler/ProfilerSample.h"

class ProfilerWindow : public IWindow
{
public:
	ProfilerWindow(const Ref<Engine>& engine, AVulkan::GraphicsApiVulkan& vulkanApi);
	void Update() override;

private:
	Ref<Engine> engine;
	AVulkan::GraphicsApiVulkan& vulkanApi;

	void ShowRecursive(ProfilerSample& sample) const;
};
