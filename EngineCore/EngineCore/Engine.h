#pragma once

#include <vector>
#include <chrono>
#include <entt.hpp>

#include "Entities/Level.h"

#include "EngineCore/AssetsDatabase.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"

#include "EngineCore/Ref.h"
#include "EngineCore/Ecs.h"
#include "EngineCore/ProjectSettings.h"
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

private:
	const Ref<ProjectSettigns> projectSettings;

	GLFWwindow* window;
	IGraphicsApi* graphicsApi;
	Level* level;
	
	Ref<Ecs> ecs;
	Ref<AssetsDatabase> assetsDatabase;

	Rollback* engineRollback;

	std::function<void()> editorUpdate;

	std::chrono::steady_clock::time_point cachedTime;

	void InitLogger();
	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();
	void InitGraphicsApi();
};

