#pragma once

#include <entt.hpp>
#include "Defines/DllDecDefines.h"

#include "AssetsDatabase.h"
#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Entities/Level.h"

#include "ProjectSettings.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Rollback/Rollback.h"

class Engine
{
public:
	Engine(Ref<ProjectSettigns> projectSettings);
	virtual ~Engine();

private:
	const Ref<ProjectSettigns> projectSettings;

	GLFWwindow* window;
	IGraphicsApi* graphicsApi;
	Level* level;

	Ref<entt::registry> ecs;
	Ref<AssetsDatabase> assetsDatabase;

	Rollback* engineRollback;

	void Run();

	void InitLogger();
	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();
	void InitGraphicsApi();
	void SubscribeGraphicsApiEvents();

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

