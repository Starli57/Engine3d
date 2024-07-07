#pragma once

#include <imgui.h>

#include "Inspector.h"

#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/IWindow.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector);
	~Hierarchy();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<Inspector> inspector;
};

