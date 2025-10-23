#include "EngineCore/Pch.h"

#include "EngineApi.h"

#include "CustomAssert.h"
#include "Profiler/Profiler.h"

namespace Engine
{
	EngineApi::EngineApi(const Ref<ProjectSettings>& projectSettings)
	{
		InitLogger();

		cachedTime = std::chrono::high_resolution_clock::now();
		engineContext = new EngineContext(projectSettings);
		engineContext->ecs = CreateRef<Ecs>();
		engineContext->systemsState = CreateRef<SystemsState>();
	
#if GLFW_INCLUDE_VULKAN
		engineContext->resourcesStorage = CreateRef<ResourcesStorageVulkan>(projectSettings);
#else
		throw std::runtime_error("The rendering api is not supported");
#endif

		engineContext->entitySerializer = CreateRef<EntitySerializer>(projectSettings, engineContext->resourcesStorage);
	
		spdlog::info("--Engine init finished--");
	}

	EngineApi::~EngineApi()
	{
		engineContext->resourcesManager->UnLoadAllMaterial();
		engineContext->resourcesManager->UnLoadAllMeshes();
		engineContext->resourcesManager->UnLoadAllTextures();
		engineContext->resourcesManager.reset();
		engineContext->resourcesStorage.reset();

		spdlog::info("Dispose renderer");
		delete engineContext->renderer;

		spdlog::info("Dispose glfw window");
		glfwDestroyWindow(engineContext->window);
		delete engineContext;
		
		glfwTerminate();
	}

	void EngineApi::InitLogger() const
	{
#ifdef DEBUG
		spdlog::set_level(spdlog::level::debug);
#else
		spdlog::set_level(spdlog::level::info);
#endif

		spdlog::set_pattern("[%^%l%$] %v");

	}

	void EngineApi::InitGlfw() const
	{
		auto status = glfwInit();
		CAssert::Check(status == GLFW_TRUE, "Glfw can't be init");
	}

	void EngineApi::SetupGlfwHints() const
	{
#if GLFW_INCLUDE_VULKAN
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
	}

	void EngineApi::CreateAppWindow()
	{
		engineContext->window = glfwCreateWindow(1600, 1200, engineContext->projectSettings->projectName.c_str(), nullptr, nullptr);
		CAssert::Check(engineContext->window != nullptr, "GLFW window can't be created");

		glfwSetWindowUserPointer(engineContext->window, this);
	}

	void EngineApi::DefineInput()
	{
		engineContext->input = CreateRef<InputManager>(engineContext->window, engineContext->ecs);
	}

	void EngineApi::DefineGraphicsApi()
	{
#if GLFW_INCLUDE_VULKAN
		engineContext->renderer = new RendererVulkan(engineContext->ecs, engineContext->input, engineContext->resourcesStorage, engineContext->projectSettings, engineContext->window);
#else
		throw std::runtime_error("The rendering api is not supported");
#endif
	}

	void EngineApi::DefineResourcesManager()
	{
#if GLFW_INCLUDE_VULKAN
		engineContext->resourcesManager = CreateRef<AssetsLoaderVulkan>(engineContext->projectSettings, engineContext->renderer, engineContext->resourcesStorage);
#else
		throw std::runtime_error("The rendering api is not supported");
#endif
	}

	void EngineApi::Run()
	{
	
		while (!glfwWindowShouldClose(engineContext->window))
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration<float>(currentTime - cachedTime).count();
		
			engineContext->resourcesManager->Load();
			glfwPollEvents();
		
			Profiler::GetInstance().BeginSample("Systems");
			engineContext->input->Update();
			gameSystemsUpdate();
			Profiler::GetInstance().EndSample();

			Profiler::GetInstance().BeginSample("Rendering");
			engineContext->renderer->Render();
			Profiler::GetInstance().EndSample();

#if _DEBUG
			if (editorUpdate) editorUpdate();
#endif
			
			Profiler::GetInstance().Reset();
		
			uint32_t targetDeltaMs = 16;
			uint32_t frameDeltaTime = static_cast<uint32_t>(std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - currentTime).count() * 1000);
			if (targetDeltaMs > frameDeltaTime)
			{
				;// Sleep(targetDeltaMs - frameDeltaTime);
			}
		
			cachedTime = currentTime;
		}

		engineContext->renderer->FinalizeRenderOperations();
		spdlog::info("Window closed");
	}

	void EngineApi::BindGameSystemsUpdateFunction(std::function<void()> gameSystemsUpdate)
	{
		this->gameSystemsUpdate = gameSystemsUpdate;
	}

	void EngineApi::BindEditorUpdateFunction(std::function<void()> editorUpdate)
	{
		this->editorUpdate = editorUpdate;
	}
}