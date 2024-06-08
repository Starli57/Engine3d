#pragma once

#include <imgui.h>

#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/IWindow.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(Ref<Ecs> ecs);
	~Hierarchy();

	void Update() override;

private:
	Ref<Ecs> ecs;

	int selectedItemIndex = 0;
};

