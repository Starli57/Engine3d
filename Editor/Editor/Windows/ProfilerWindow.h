#pragma once

#include <imgui.h>
#include <vector>

#include "EngineCore/EngineApi.h"
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Profiler/ProfilerSample.h"

class ProfilerWindow : public Engine::IWindow
{
public:
	ProfilerWindow(Engine::EngineContext* engineContext);
	void Update() override;

private:
	Engine::EngineContext* engineContext;
	void ShowRecursive(Engine::ProfilerSample& sample) const;
};
