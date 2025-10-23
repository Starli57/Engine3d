#include "Game.h"

Game::Game(Ref<ProjectSettings> projectSettings) : projectSettings(projectSettings)
{
	engine = CreateRef<Engine::EngineApi>(projectSettings);
	engine->InitGlfw();
	engine->SetupGlfwHints();
	engine->CreateAppWindow();
	engine->DefineInput();
	engine->DefineGraphicsApi();

	auto& engineContext = engine->engineContext;
	
#if GLFW_INCLUDE_VULKAN
	renderer = CreateRef<ClientVulkanApi::RendererVulkan>(engineContext);
	engineContext->renderer->Init();
#endif
	
	engine->DefineResourcesManager();
	engine->BindGameSystemsUpdateFunction([this]() { UpdateGameSystems(); });
	
	serializer = CreateRef<Client::EntitySerializer>(engineContext->entitySerializer);
	
	InitializeGameSystems();
}

Game::~Game()
{
	engine.reset();
}

void Game::Run() const
{
	engine->Run();
}

void Game::InitializeGameSystems()
{
	auto window = engine->engineContext->window;
	auto ecs = engine->engineContext->ecs;
	auto input = engine->engineContext->input;
	auto systemsState = engine->engineContext->systemsState;
	
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
