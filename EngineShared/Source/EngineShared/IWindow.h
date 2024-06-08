#pragma once

#include <entt.hpp>

#include "EngineShared/Ref.h"

class IWindow
{
public:
	virtual void Update() = 0;
};