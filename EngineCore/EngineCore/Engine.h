#pragma once

#include <vector>
#include <chrono>
#include <entt.hpp>

#include "Entities/Level.h"

#include "EngineCore/AssetsManagement/AssetsDatabase.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"

class Engine
{
public:
	Engine(Ref<ProjectSettigns> projectSettings);
	virtual ~Engine();

	void Run();

	void BindEditorUpdateFunction(std::function<void()> editorUpdate);

	IGraphicsApi* GetGraphicsApi() { return graphicsApi; }
	Ref<Ecs> GetEcs() { return ecs; }
	Ref<AssetsDatabase> GetAssetsDatabase() { return assetsDatabase; }

	float GetDeltaTime() { return deltaTime; }

private:
	const Ref<ProjectSettigns> projectSettings;

	GLFWwindow* window;
	IGraphicsApi* graphicsApi;
	Level* level;
	
	Ref<Ecs> ecs;
	Ref<AssetsDatabase> assetsDatabase;

	Rollback* engineRollback;

	float deltaTime;

	std::function<void()> editorUpdate;

	std::chrono::steady_clock::time_point cachedTime;

	void InitLogger();
	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();
	void InitGraphicsApi();
};

