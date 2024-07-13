#pragma once

#include <vector>
#include <chrono>
#include <entt.hpp>

#include "AssetsDatabase.h"
#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Entities/Level.h"
#include "Systems/MeshLoaderSystem.h"

#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/ProjectSettings.h"
#include "EngineShared/Rollback/Rollback.h"

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

