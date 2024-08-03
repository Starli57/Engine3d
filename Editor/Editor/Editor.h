#pragma once

#include "IEngineEditor.h"

#include "ExampleProject/Game.h"
#include "EngineCore/Engine.h"
#include "EngineCore/Core/Ref.h"

class Editor
{
public:
	Editor();
	~Editor();

private:
	Ref<Game> game;
	Ref<Engine> engine;
	Ref<IEngineEditor> editorUi;
	Ref<ProjectSettigns> projectSettings;
};

