#pragma once

#include "Defines/DllDecDefines.h"

#include "Rendering/Renderer.h"
#include "Base/Level.h"

#include "Architecture/Rollback/Rollback.h"

class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run();

private:
	Renderer* renderer;

	Rollback* engineRollback;

	Level* level;

	void InitLogger();
};

