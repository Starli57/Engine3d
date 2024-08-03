#pragma once

#include <imgui.h>

#include "Inspector.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Editor/IWindow.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector);
	~Hierarchy();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<Inspector> inspector;

	int selectedItemIndex;
};

