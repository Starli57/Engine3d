#pragma once

#include <imgui.h>

#include "EngineCore/Editor/IWindow.h"

class ImguiDemo : public EngineCore::IWindow
{
public:
	void Update() override;
};