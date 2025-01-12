#pragma once

#include <vector>
#include <chrono>
#include <entt.hpp>

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Core/World.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Input.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Systems/CameraFlySystem.h"
#include "EngineCore/Systems/CameraOrbitSystem.h"
#include "EngineCore/Systems/RotatorSystem.h"

#include "EngineCore/Rollback/Rollback.h"

#if GLFW_INCLUDE_VULKAN
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/AssetsLoaderVulkan.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#endif

class Engine
{
public:
	Engine(Ref<ProjectSettings> projectSettings);
	virtual ~Engine();

	void Run();

	void BindEditorUpdateFunction(std::function<void()> editorUpdate);

	IGraphicsApi* GetGraphicsApi() { return graphicsApi; }
	Ref<Ecs> GetEcs() { return ecs; }
	
	Ref<AssetsDatabase> GetAssetsDatabase() { return assetsDatabase; }
	Ref<AssetsLoader> GetResourcesManager() { return resourcesManager; }

	Ref<World> GetLevel() { return world; }

	float GetDeltaTime() { return deltaTime; }

private:
	const Ref<ProjectSettings> projectSettings;

	GLFWwindow* window;
	IGraphicsApi* graphicsApi;

	Ref<World> world;
	Ref<Ecs> ecs;
	Ref<Input> input;
	Ref<AssetsDatabase> assetsDatabase;
	Ref<AssetsLoader> resourcesManager;

	float deltaTime;

	std::function<void()> editorUpdate;

	std::chrono::steady_clock::time_point cachedTime;

	void InitLogger();
	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();
	void InitGraphicsApi();
};

