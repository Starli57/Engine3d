#include "Game.h"

Game::Game(Ref<ProjectSettings> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine>(projectSettings);
	editor = CreateRef<Editor>(projectSettings, engine);

	assetDatabase = engine->GetAssetsDatabase();
	resourcesManager = engine->GetResourcesManager();
	graphicsApi = engine->GetGraphicsApi();
	ecs = engine->GetEcs();

}

Game::~Game()
{
	editor.reset();
	engine.reset();
}

void Game::Run() 
{
	engine->Run();
}
