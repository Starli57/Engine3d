#include "EngineCore/Pch.h"

#include "Engine.h"

#include "CustomAssert.h"
#include "Profiler/Profiler.h"

namespace EngineCore
{
	Engine::Engine(const Ref<ProjectSettings>& projectSettings) : projectSettings(projectSettings)
	{
		InitLogger();

		cachedTime = std::chrono::high_resolution_clock::now();
		ecs = CreateRef<Ecs>();
		systemsState = CreateRef<SystemsState>();
	
#if GLFW_INCLUDE_VULKAN
		assetsDatabase = CreateRef<ResourcesStorageVulkan>(projectSettings);
#else
		throw std::runtime_error("The rendering api is not supported");
#endif

		entitySerializer = CreateRef<EngineCore::EntitySerializer>(projectSettings,assetsDatabase);
	
		spdlog::info("--Engine init finished--");
	}

	Engine::~Engine()
	{
		resourcesManager->UnLoadAllMaterial();
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
		window = glfwCreateWindow(1600, 1200, projectSettings->projectName.c_str(), nullptr, nullptr);
		CAssert::Check(window != nullptr, "GLFW window can't be created");

		glfwSetWindowUserPointer(window, this);
	}

	void Engine::DefineInput()
	{
		input = CreateRef<InputManager>(window, ecs);
	}

	void Engine::DefineGraphicsApi()
	{
#if GLFW_INCLUDE_VULKAN
		graphicsApi = new AVulkan::GraphicsApiVulkan(ecs, input, assetsDatabase, projectSettings, window);
#else
		throw std::runtime_error("The rendering api is not supported");
#endif
	}

	void Engine::DefineResourcesManager()
	{
#if GLFW_INCLUDE_VULKAN
		resourcesManager = CreateRef<AssetsLoaderVulkan>(projectSettings, graphicsApi, assetsDatabase);
#else
		throw std::runtime_error("The rendering api is not supported");
#endif
	}

	void Engine::Run()
	{
	
		while (!glfwWindowShouldClose(window))
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration<float>(currentTime - cachedTime).count();
		
			resourcesManager->Load();
			glfwPollEvents();
		
			Profiler::GetInstance().BeginSample("Systems");
			input->Update();
			gameSystemsUpdate();
			Profiler::GetInstance().EndSample();

			Profiler::GetInstance().BeginSample("Rendering");
			graphicsApi->Render();
			Profiler::GetInstance().EndSample();
		
			editorUpdate();
			Profiler::GetInstance().Reset();
		
			uint32_t targetDeltaMs = 16;
			uint32_t frameDeltaTime = static_cast<uint32_t>(std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - currentTime).count() * 1000);
			if (targetDeltaMs > frameDeltaTime)
			{
				Sleep(targetDeltaMs - frameDeltaTime);
			}
		
			cachedTime = currentTime;
		}

		graphicsApi->FinalizeRenderOperations();
		spdlog::info("Window closed");
	}

	void Engine::BindGameSystemsUpdateFunction(std::function<void()> gameSystemsUpdate)
	{
		this->gameSystemsUpdate = gameSystemsUpdate;
	}

	void Engine::BindEditorUpdateFunction(std::function<void()> editorUpdate)
	{
		this->editorUpdate = editorUpdate;
	}
}