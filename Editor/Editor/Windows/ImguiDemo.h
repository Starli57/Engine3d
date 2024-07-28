#pragma once

#include <imgui.h>

#include "EngineCore/IWindow.h"

class ImguiDemo : public IWindow
{
public:
	void Update() override;
};