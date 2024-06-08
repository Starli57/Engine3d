#pragma once

#include <imgui.h>

#include "SharedLib/IWindow.h"

class ImguiDemo : public IWindow
{
public:
	void Update() override;
};