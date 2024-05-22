#pragma once

#include "Engine.h"
#include "IEngineEditor.h"

#include "SharedLib/Ref.h"

class Editor
{
public:
	Editor();
	~Editor();

private:
	Ref<Engine> engine;
	Ref<IEngineEditor> editorUi;
	Ref<ProjectSettigns> projectSettings;
};

