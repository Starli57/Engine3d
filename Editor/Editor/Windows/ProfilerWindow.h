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
	ProfilerWindow(const Ref<Engine::EngineApi>& engine, Engine::RendererVulkan& renderer);
	void Update() override;

private:
	Ref<Engine::EngineApi> engine;
	Engine::RendererVulkan& renderer;

	void ShowRecursive(Engine::ProfilerSample& sample) const;
};
