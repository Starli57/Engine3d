#pragma once

#include <imgui.h>

#include "EngineCore/Editor/IWindow.h"

class ImguiDemo : public Engine::IWindow
{
public:
	void Update() override;
};