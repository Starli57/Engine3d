#pragma once

#include <imgui.h>
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/AssetsDatabase.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Serialization/EntitySerializer.h"

class AssetsWindow : public IWindow
{
public:
	AssetsWindow(const Ref<Ecs>& ecs, const Ref<AssetsDatabase>& assetsDatabase, const Ref<ProjectSettings>& projectSettings);
	~AssetsWindow();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<AssetsDatabase> assetsDatabase;
	Ref<ProjectSettings> projectSettings;
};

