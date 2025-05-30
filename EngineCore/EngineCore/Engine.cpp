#include "EngineCore/Pch.h"

#include "Engine.h"

#include "Profiler/Profiler.h"

Engine::Engine(const Ref<ProjectSettings>& projectSettings) : projectSettings(projectSettings)
{
	InitLogger();

	cachedTime = std::chrono::high_resolution_clock::now();
	ecs = CreateRef<Ecs>();

#if GLFW_INCLUDE_VULKAN
	assetsDatabase = CreateRef<AssetsDatabaseVulkan>(projectSettings);
#else
	throw std::runtime_error("The rendering api is not supported");
#endif

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
		throw;
	}


#if GLFW_INCLUDE_VULKAN
	resourcesManager = CreateRef<AssetsLoaderVulkan>(projectSettings, graphicsApi, assetsDatabase);
#else
	throw std::runtime_error("The rendering api is not supported");
#endif

	input = CreateRef<Input>(window);

	spdlog::info("--Engine init finished--");
}

Engine::~Engine()
{
	resourcesManager->UnLoadAllMeshes();
	resourcesManager->UnLoadAllTextures();
	resourcesManager.reset();
	assetsDatabase.reset();

	spdlog::info("Dispose graphicsApi");
	delete graphicsApi;

	spdlog::info("Dispose glfw window");
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::InitLogger() const
{
#ifdef DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	spdlog::set_pattern("[%^%l%$] %v");

}

void Engine::InitGlfw() const
{
	auto status = glfwInit();
	CAssert::Check(status == GLFW_TRUE, "Glfw can't be init");
}

void Engine::SetupGlfwHints() const
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
	Ref<AssetsDatabaseVulkan> databaseVulkan = std::dynamic_pointer_cast<AssetsDatabaseVulkan>(assetsDatabase);
	graphicsApi = new AVulkan::GraphicsApiVulkan(ecs, databaseVulkan, projectSettings, window);
	graphicsApi->Init();
#else
	throw std::runtime_error("The rendering api is not supported");
#endif
}

void Engine::Run()
{
	const auto rotatorSystem = CreateRef<RotatorSystem>(ecs);
	const auto transformSystem = CreateRef<TransformSystem>(ecs);
	const auto cameraSystem = CreateRef<ViewProjectionSystem>(ecs, window);
	const auto freeCameraSystem = CreateRef<CameraFlySystem>(ecs, input);
	const auto orbitCameraSystem = CreateRef<CameraOrbitSystem>(ecs, input);

	while (!glfwWindowShouldClose(window))
	{
		Profiler::GetInstance().BeginSample("MainLoop");
		
		glfwPollEvents();
		input->Update();

		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(currentTime - cachedTime).count();

		resourcesManager->Load();
		
		Profiler::GetInstance().BeginSample("Systems");
		rotatorSystem->Update(deltaTime);
		transformSystem->Update(deltaTime);
		freeCameraSystem->Update(deltaTime);
		orbitCameraSystem->Update(deltaTime);
		cameraSystem->Update(deltaTime);
		Profiler::GetInstance().EndSample("Systems");

		Profiler::GetInstance().BeginSample("Rendering");
		graphicsApi->Render();
		Profiler::GetInstance().EndSample("Rendering");

		Profiler::GetInstance().EndSample("MainLoop");

		editorUpdate();

		cachedTime = currentTime;
	}

	graphicsApi->FinalizeRenderOperations();
	spdlog::info("Window closed");
}

void Engine::BindEditorUpdateFunction(std::function<void()> editorUpdate)
{
	this->editorUpdate = editorUpdate;
}
