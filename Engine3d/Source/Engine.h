#pragma once

#include "Defines/DllDecDefines.h"
#include "Rendering/Renderer.h"

class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run();

private:
	Renderer* renderer;
};

