#pragma once

#include <imgui.h>

#include "EngineShared/IWindow.h"

class ImguiDemo : public IWindow
{
public:
	void Update() override;
};