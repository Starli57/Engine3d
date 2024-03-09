#pragma once

#include "AssetsDatabase.h"
#include "IGraphicsApi.h"
#include "Vulkan/VulkanGraphicsApi.h"
#include "SharedLib/Rollback/Rollback.h"
#include "SharedLib/Ref.h"
#include "Entities/Level.h"

using namespace AVulkan;

class Renderer
{
public:
	//todo: use getter
	GLFWwindow* window;

	IGraphicsApi* GetGraphicsApi() { return graphicsApi; }

	Renderer(Ref<entt::registry> ecs, Ref<AssetsDatabase> assetsDatabase, Rollback& engineRollback);
	~Renderer();

	void Init();
	void Run();

private: 
	Ref<entt::registry> ecs;
	Ref<AssetsDatabase> assetsDatabase;

	IGraphicsApi* graphicsApi;

	Rollback* rollback;

	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();
	void InitGraphicsApi();
	void SubscribeGraphicsApiEvents();

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

