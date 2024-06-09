#pragma once

#include <vector>
#include <chrono>
#include <entt.hpp>
#include "Defines/DllDecDefines.h"

#include "IEngineEditor.h"
#include "AssetsDatabase.h"
#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Entities/Level.h"

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

	void BindEditor(Ref<IEngineEditor> editor);

	IGraphicsApi* GetGraphicsApi() { return graphicsApi; }
	Ref<Ecs> GetEcs() { return ecs; }

private:
	const Ref<ProjectSettigns> projectSettings;

	GLFWwindow* window;
	IGraphicsApi* graphicsApi;
	Level* level;
	
	Ref<Ecs> ecs;
	Ref<IEngineEditor> editor;
	Ref<AssetsDatabase> assetsDatabase;

	Rollback* engineRollback;

	std::chrono::steady_clock::time_point cachedTime;

	void InitLogger();
	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();
	void InitGraphicsApi();
};
