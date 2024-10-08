#include "EngineCore/Pch.h"

#include "Engine.h"

Engine::Engine(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	InitLogger();

	cachedTime = std::chrono::high_resolution_clock::now();
	engineRollback = new Rollback("Engine");

	ecs = CreateRef<Ecs>();

	assetsDatabase = CreateRef<AssetsDatabase>(projectSettings);
	engineRollback->Add([this]() { assetsDatabase.reset(); });

	try
	{
		InitGlfw();
		SetupGlfwHints();
		CreateAppWindow();
		InitGraphicsApi();
	}
	catch (const std::exception& e)
	{
		spdlog::critical("Renderer critical error: {0}", e.what());
		engineRollback->Dispose();
		throw e;
	}

	resourcesManager = CreateRef<ResourcesManager>(graphicsApi, assetsDatabase);
	engineRollback->Add([this]() { resourcesManager.reset(); });

	input = CreateRef<Input>(window);

	world = CreateRef<World>(ecs, projectSettings);
	engineRollback->Add([this] { world.reset(); });

	spdlog::info("--Engine init finished--");
}

Engine::~Engine()
{
	delete engineRollback;
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

void Engine::InitGlfw()
{
	auto status = glfwInit();
	CAssert::Check(status == GLFW_TRUE, "Glfw can't be init");
}

void Engine::SetupGlfwHints()
{
#if GLFW_INCLUDE_VULKAN
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
}

void Engine::CreateAppWindow()
{
	window = glfwCreateWindow(1600, 1000, projectSettings->projectName.c_str(), nullptr, nullptr);
	CAssert::Check(window != nullptr, "GLFW window can't be created");

	engineRollback->Add([this]()
	{
		spdlog::info("Dispose glfw window");
		glfwDestroyWindow(window);
		glfwTerminate();
	});

	glfwSetWindowUserPointer(window, this);
}

void Engine::InitGraphicsApi()
{
#if GLFW_INCLUDE_VULKAN
	graphicsApi = new AVulkan::GraphicsApiVulkan(ecs, assetsDatabase, projectSettings, window, engineRollback);
	graphicsApi->Init();
	engineRollback->Add([this] { delete graphicsApi; });
#else
	throw std::runtime_error("Rendering api is not selected");
#endif
}

void Engine::Run()
{
	auto rotatorSystem = CreateRef<RotatorSystem>(ecs);
	auto transformSystem = CreateRef<TransformSystem>(ecs);
	auto cameraSystem = CreateRef<Camera>(ecs, window);
	auto freeCameraSystem = CreateRef<CameraFlySystem>(ecs, input);
	auto orbitCameraSystem = CreateRef<CameraOrbitSystem>(ecs, input);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		input->Update();

		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - cachedTime).count();

		rotatorSystem->Update(deltaTime);
		transformSystem->Update(deltaTime);
		freeCameraSystem->Update(deltaTime);
		orbitCameraSystem->Update(deltaTime);
		cameraSystem->Update(deltaTime);

		graphicsApi->Render();

		editorUpdate();

		cachedTime = currentTime;
	}

	graphicsApi->FinanilizeRenderOperations();
	spdlog::info("Window closed");
}

void Engine::BindEditorUpdateFunction(std::function<void()> editorUpdate)
{
	this->editorUpdate = editorUpdate;
}
