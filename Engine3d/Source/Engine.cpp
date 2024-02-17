#include "Pch.h"

#include <spdlog/spdlog.h>
#include "entt.hpp"

#include "Engine.h"

Engine::Engine()
{
	InitLogger();

	engineRollback = new Rollback("Engine");

	ecs = CreateRef<entt::registry>();

	level = new Level(ecs, engineRollback);
	level->LoadLevel();
	engineRollback->Add([this] { delete level; });

	renderer = new Renderer(ecs, *engineRollback);
	renderer->Init();
	engineRollback->Add([this] { delete renderer; });

	spdlog::info("--Engine init finished--");
}

Engine::~Engine()
{
	delete engineRollback;
}

void Engine::Run()
{
	renderer->Run();
}

void Engine::InitLogger()
{
#ifdef DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	spdlog::set_pattern("[%^%l%$] %v");

}
