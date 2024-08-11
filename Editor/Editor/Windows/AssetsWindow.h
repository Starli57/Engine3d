#pragma once

#include <imgui.h>
#include "EngineCore/Editor/IWindow.h"
#include <EngineCore/AssetsManagement/AssetsDatabase.h>
#include <EngineCore/Entities/Level.h>

class AssetsWindow : public IWindow
{
public:
	AssetsWindow(Ref<AssetsDatabase> assetsDatabase, Level* level);
	~AssetsWindow();

	void Update() override;

private:
	Ref<AssetsDatabase> assetsDatabase;
	Level* level;
};

