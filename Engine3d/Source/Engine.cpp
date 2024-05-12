#include "Pch.h"

#include <spdlog/spdlog.h>

#include "Engine.h"

Engine::Engine(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	InitLogger();

	cachedTime = std::chrono::high_resolution_clock::now();
	engineRollback = new Rollback("Engine");

	ecs = CreateRef<entt::registry>();

	assetsDatabase = CreateRef<AssetsDatabase>();
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

	level = new Level(ecs, projectSettings, assetsDatabase, graphicsApi, engineRollback);
	level->LoadLevel();
	engineRollback->Add([this] { delete level; });

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
	window = glfwCreateWindow(1000, 1000, "Engine window", nullptr, nullptr);
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
	graphicsApi = new AVulkan::VulkanGraphicsApi(ecs, projectSettings, window, engineRollback);
	graphicsApi->Init();
	engineRollback->Add([this] { delete graphicsApi; });
#else
	throw std::runtime_error("Rendering api is not selected");
#endif
}

void Engine::Run()
{
	Ref<RotatorSystem> rotatorSystem = CreateRef<RotatorSystem>(ecs);
	Ref<TransformSystem> transformSystem = CreateRef<TransformSystem>(ecs);
	Ref<Camera> cameraSystem = CreateRef<Camera>(ecs, window, 60);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - cachedTime).count();

		rotatorSystem->Update(deltaTime);
		transformSystem->Update(deltaTime);
		cameraSystem->Update(deltaTime);

		//todo: handle exceptions and errors
		graphicsApi->Render();


		cachedTime = currentTime;
	}

	graphicsApi->FinanilizeRenderOperations();
	spdlog::info("Window closed");
}

void Engine::BindEditor(Ref<IEngineEditor> editor)
{
	this->editor = editor;
	graphicsApi->BindEditor(editor);
}
