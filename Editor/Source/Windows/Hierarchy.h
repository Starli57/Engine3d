#pragma once

#include <imgui.h>

#include "EngineShared/IWindow.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(Ref<entt::registry> ecs);
	~Hierarchy();

	void Update() override;

private:
	Ref<entt::registry> ecs;

	int selectedItemIndex;
};

