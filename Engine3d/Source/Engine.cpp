#include "Pch.h"
#include "Engine.h"

Engine::Engine()
{
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
