#include "Pch.h"
#include "Engine.h"

Engine::Engine()
{
	InitLogger();

	engineRollback = new Rollback();

	level = new Level(engineRollback);
	level->LoadLevel();

	renderer = new Renderer(engineRollback, level);
	renderer->Init();
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
