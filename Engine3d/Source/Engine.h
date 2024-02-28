#pragma once

#include <entt.hpp>
#include "Defines/DllDecDefines.h"

#include "Architecture/Ref.h"
#include "Rendering/Renderer.h"
#include "Entities/Level.h"

#include "Architecture/Rollback/Rollback.h"

class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	void Run();

private:
	Renderer* renderer;
	Level* level;

	Rollback* engineRollback;


	Ref<entt::registry> ecs;
	Ref<AssetsDatabaseVulkan> assetsDatabase;

	void InitLogger();
};

