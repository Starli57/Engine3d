#pragma once

#include "Engine.h"
#include "IEngineEditor.h"

#include "Imgui/ImguiVulkan.h"

#include "SharedLib/Ref.h"

class Editor
{
public:
	Editor();
	~Editor();

private:
	Ref<Engine> engine;
	Ref<ImguiVulkan> imgui;
	Ref<ProjectSettigns> projectSettings;
};

