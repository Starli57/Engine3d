#pragma once

#include "IEngineEditor.h"

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Engine.h"
#include "EngineCore/Core/Ref.h"

class Editor
{
public:
	Editor(const Ref<ProjectSettings>& projectSettings, const Ref<Engine>& engine);
	~Editor();

private:
	Ref<IEngineEditor> editorUi;
	const Ref<ProjectSettings>& projectSettings;
	const Ref<Engine>& engine;
};

