#include "Game.h"

Game::Game(Ref<ProjectSettings> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine::EngineApi>(projectSettings);
	engine->InitGlfw();
	engine->SetupGlfwHints();
	engine->CreateAppWindow();
	engine->DefineInput();
	engine->DefineGraphicsApi();
	
#if GLFW_INCLUDE_VULKAN
	auto vulkanApi = engine->GetGraphicsApi();
	renderer = CreateRef<ClientVulkanApi::RendererVulkan>(vulkanApi, engine->GetAssetsDatabase(), engine->GetEcs());
	vulkanApi->Init();
#endif
	
	engine->DefineResourcesManager();
	engine->BindGameSystemsUpdateFunction([this]() { UpdateGameSystems(); });
	
	serializer = CreateRef<Client::EntitySerializer>(engine->GetSerializer());

#if DEBUG
	editor = CreateRef<Editor>(projectSettings, engine);
#endif
	
#if !DEBUG
	engine->GetSerializer()->InstantiateWorld(engine->GetEcs(), engine->GetResourcesManager()->assetsDatabase->worldsPaths.at(0));
#endif
	
	InitializeGameSystems();
}

Game::~Game()
{
#if DEBUG
	editor.reset();
#endif
	
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
	cameraProjectionSystem = CreateUniqueRef<ViewProjectionSystem>(ecs, window);
	directionalShadowMapProjectionSystem = CreateUniqueRef<DirectionalShadowMapProjectionSystem>(ecs, window);
	freeCameraSystem = CreateUniqueRef<CameraFlySystem>(ecs, input);
}

void Game::UpdateGameSystems() const
{
	auto deltaTime = engine->GetDeltaTime();
	cameraLookAtPositionSystem->Update(deltaTime);
	rotatorSystem->Update(deltaTime);
	transformSystem->Update(deltaTime);
	freeCameraSystem->Update(deltaTime);
	cameraProjectionSystem->Update(deltaTime);
	directionalShadowMapProjectionSystem->Update(deltaTime);
}
