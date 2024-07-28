#pragma once

#include <imgui.h>

#include "Inspector.h"

#include "EngineCore/Ref.h"
#include "EngineCore/Ecs.h"
#include "EngineCore/IWindow.h"

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

