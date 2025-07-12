#include "Game.h"

Game::Game(Ref<ProjectSettings> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine>(projectSettings);
	engine->InitGlfw();
	engine->SetupGlfwHints();
	engine->CreateAppWindow();
	engine->DefineInput();
	engine->DefineGraphicsApi();
	
#if GLFW_INCLUDE_VULKAN
	auto vulkanApi = engine->GetGraphicsApi();
	renderer = CreateRef<AVulkan::RendererVulkan>(vulkanApi, engine->GetAssetsDatabase(), engine->GetEcs());
	vulkanApi->Init();
#endif
	
	engine->DefineResourcesManager();
	engine->BindGameSystemsUpdateFunction([this]() { UpdateGameSystems(); });
	
	serializer = CreateRef<Client::EntitySerializer>(engine->GetSerializer());
	
	editor = CreateRef<Editor>(projectSettings, engine);
	
	InitializeGameSystems();
}

Game::~Game()
{
	editor.reset();
	engine.reset();
}

void Game::Run() const
{
	engine->Run();
}

void Game::InitializeGameSystems()
{
	auto window = engine->GetWindow();
	auto ecs = engine->GetEcs();
	auto input = engine->GetInput();
	auto systemsState = engine->GetSystemsState();
	
	cameraLookAtPositionSystem = CreateUniqueRef<CameraLookAtSystem>(ecs, input);
	rotatorSystem = CreateUniqueRef<RotateSystem>(ecs);
	transformSystem = CreateUniqueRef<TransformSystem>(ecs);
	cameraSystem = CreateUniqueRef<ViewProjectionSystem>(ecs, window);
	freeCameraSystem = CreateUniqueRef<CameraFlySystem>(ecs, input);
}

void Game::UpdateGameSystems() const
{
	auto deltaTime = engine->GetDeltaTime();
	cameraLookAtPositionSystem->Update(deltaTime);
	rotatorSystem->Update(deltaTime);
	transformSystem->Update(deltaTime);
	freeCameraSystem->Update(deltaTime);
	cameraSystem->Update(deltaTime);
}
