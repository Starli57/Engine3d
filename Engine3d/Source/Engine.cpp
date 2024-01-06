#include "Pch.h"
#include "Engine.h"

Engine::Engine()
{
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
