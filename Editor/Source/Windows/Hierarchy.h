#pragma once

#include <imgui.h>

#include "SharedLib/IWindow.h"

class Hierarchy : public IWindow
{
public:
	void Update() override;
};

