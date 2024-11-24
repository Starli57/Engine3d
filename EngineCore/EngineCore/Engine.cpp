#include "EngineCore/Pch.h"

#include "Engine.h"

Engine::Engine(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	InitLogger();

	cachedTime = std::chrono::high_resolution_clock::now();
	ecs = CreateRef<Ecs>();
	assetsDatabase = CreateRef<AssetsDatabase>(projectSettings);

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
		throw e;
	}


	resourcesManager = CreateRef<ResourcesManager>(graphicsApi, assetsDatabase);
	input = CreateRef<Input>(window);
	world = CreateRef<World>(ecs, projectSettings);

	spdlog::info("--Engine init finished--");
}

Engine::~Engine()
{
	world.reset();
	resourcesManager.reset();

	assetsDatabase->Dispose(); 
	assetsDatabase.reset();

	spdlog::info("Dispose graphicsApi");
	delete graphicsApi;

	spdlog::info("Dispose glfw window");
	glfwDestroyWindow(window);
	glfwTerminate();
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

	glfwSetWindowUserPointer(window, this);
}

void Engine::InitGraphicsApi()
{
#if GLFW_INCLUDE_VULKAN
	graphicsApi = new AVulkan::GraphicsApiVulkan(ecs, assetsDatabase, projectSettings, window);
	graphicsApi->Init();
#else
	throw std::runtime_error("Rendering api is not selected");
#endif
}

void Engine::Run()
{
	auto rotatorSystem = CreateRef<RotatorSystem>(ecs);
	auto transformSystem = CreateRef<TransformSystem>(ecs);
	auto cameraSystem = CreateRef<ViewProjectionSystem>(ecs, window);
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
