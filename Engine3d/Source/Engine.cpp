#include "Pch.h"

#include <spdlog/spdlog.h>

#include "Engine.h"

Engine::Engine(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
	InitLogger();

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
		SubscribeGraphicsApiEvents();
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
	glfwSetFramebufferSizeCallback(window, OnFramebufferResized);
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

void Engine::SubscribeGraphicsApiEvents()
{
	auto cameraScreenRationHandler = [this](float aspectRation)
	{
		//todo: replace the logic to cameraSystem
		auto cameraEntities = ecs->view<Camera>();
		for (auto entity : cameraEntities)
		{
			auto camera = cameraEntities.get<Camera>(entity);
			camera.UpdateScreenAspectRatio(aspectRation);
			camera.Update();
		}
	};

	graphicsApi->OnFrameBufferAspectRatioChanged.AddHandler(cameraScreenRationHandler);

	//todo: fix subscription dispose
//	engineRollback->Add([this, handler] { graphicsApi->OnFrameBufferAspectRatioChanged.RemoveHandler(cameraScreenRationHandler); });
}

void Engine::Run()
{
	Ref<TransformSystem> transformSystem = CreateRef<TransformSystem>(ecs);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		transformSystem->Update();

		//todo: handle exceptions and errors
		graphicsApi->Render();
	}

	graphicsApi->FinanilizeRenderOperations();
	spdlog::info("Window closed");
}

void Engine::OnFramebufferResized(GLFWwindow* window, int width, int height)
{
	spdlog::debug("FramebufferResized");
	auto render = reinterpret_cast<IGraphicsApi*>(glfwGetWindowUserPointer(window));
	render->OnFramebufferResized();
}