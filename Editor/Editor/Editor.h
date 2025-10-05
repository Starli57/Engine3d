#pragma once

#include "IEngineEditor.h"

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/EngineApi.h"
#include "EngineCore/Core/Ref.h"

class Editor
{
public:
	Editor(const Ref<ProjectSettings>& projectSettings, const Ref<Engine::EngineApi>& engine);
	~Editor();

private:
	Ref<IEngineEditor> editorUi;
};

