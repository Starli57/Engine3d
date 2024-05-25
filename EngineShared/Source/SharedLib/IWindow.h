#pragma once

#include <entt.hpp>

#include "SharedLib/Ref.h"

class IWindow
{
public:
	virtual void Update() = 0;
};