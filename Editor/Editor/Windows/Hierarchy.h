#pragma once

#include <imgui.h>

#include "Inspector.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Core/AssetsDatabase.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Serialization/EntitySerializer.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector, Ref<ProjectSettings> projectSettings, Ref<AssetsDatabase> assetsDatabase);
	~Hierarchy();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<Inspector> inspector;
	Ref<EntitySerializer> serializer;
	Ref<ProjectSettings> projectSettings;
	Ref<AssetsDatabase> assetsDatabase;

	int selectedItemIndex;
};

