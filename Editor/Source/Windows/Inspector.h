#pragma once

#include <imgui.h>

#include "EngineShared/IWindow.h"

class Inspector : public IWindow
{
public:
	void Update() override;
};

