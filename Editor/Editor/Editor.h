#pragma once

#include "IEngineEditor.h"

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Engine.h"
#include "EngineCore/Core/Ref.h"

class Editor
{
public:
	Editor(Ref<ProjectSettings> projectSettings, Ref<Engine> engine);
	~Editor();

private:
	Ref<Engine> engine;
	Ref<IEngineEditor> editorUi;
	Ref<ProjectSettings> projectSettings;
};

