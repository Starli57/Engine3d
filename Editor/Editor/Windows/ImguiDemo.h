#pragma once

#include <imgui.h>

#include "EngineCore/Editor/IWindow.h"

class ImguiDemo : public IWindow
{
public:
	void Update() override;
};