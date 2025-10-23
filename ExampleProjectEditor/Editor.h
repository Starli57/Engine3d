#pragma once

#include "EngineCore/Editor/IEngineEditor.h"

#include "EngineCore/EngineApi.h"
#include "EngineCore/Core/Ref.h"

namespace UserEditor
{
	class Editor
	{
	public:
		Editor(const Ref<ProjectSettings>& projectSettings, const Ref<Engine::EngineApi>& engine);
		~Editor();

	private:
		Ref<Engine::IEngineEditor> editorUi;
	};
}