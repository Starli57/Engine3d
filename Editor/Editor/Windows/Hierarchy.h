#pragma once

#include <imgui.h>

#include "Inspector.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Serialization/EntitySerializer.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector, Ref<ProjectSettigns> projectSettings);
	~Hierarchy();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<Inspector> inspector;
	Ref<EntitySerializer> serializer;
	Ref<ProjectSettigns> projectSettings;

	int selectedItemIndex;
};

