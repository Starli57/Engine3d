#include "Pch.h"

#include <spdlog/spdlog.h>

#include "Engine.h"
#include "entt.hpp"

Engine::Engine()
{
	InitLogger();

	engineRollback = new Rollback("Engine");

	world = new World(ecs, engineRollback);
	world->LoadLevel();
	spdlog::info("World init completed");

	renderer = new Renderer(ecs, *engineRollback);
	renderer->Init();
	spdlog::info("Renderer init completed");
}

Engine::~Engine()
{
	engineRollback->Dispose();
	delete renderer;
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
