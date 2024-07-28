#include "Game.h"

Game::Game(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine>(projectSettings);
}

Game::~Game()
{
	engine.reset();
}

void Game::Run() 
{
	engine->Run();
}
