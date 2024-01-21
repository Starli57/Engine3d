#include "Pch.h"
#include "Engine.h"

Engine::Engine()
{
#ifdef DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	spdlog::set_pattern("[%^%l%$] %v");

	engineRollback = new Rollback();
	renderer = new Renderer(*engineRollback);
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
