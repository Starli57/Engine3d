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

	renderer = new Renderer();
}

Engine::~Engine()
{
	delete renderer;
}

void Engine::Run()
{
	renderer->Run();
}
