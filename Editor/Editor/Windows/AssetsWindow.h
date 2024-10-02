#pragma once

#include <imgui.h>
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/AssetsManagement/AssetsDatabase.h"
#include "EngineCore/Core/World.h"

class AssetsWindow : public IWindow
{
public:
	AssetsWindow(Ref<AssetsDatabase> assetsDatabase, Ref<World> level);
	~AssetsWindow();

	void Update() override;

private:
	Ref<AssetsDatabase> assetsDatabase;
	Ref<World> world;
};

