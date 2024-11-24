#pragma once

#include "IEngineEditor.h"

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Engine.h"
#include "EngineCore/Core/Ref.h"

class Editor
{
public:
	Editor(Ref<ProjectSettigns> projectSettings, Ref<Engine> engine);
	~Editor();

private:
	Ref<Engine> engine;
	Ref<IEngineEditor> editorUi;
	Ref<ProjectSettigns> projectSettings;
};

