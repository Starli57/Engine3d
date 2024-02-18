#pragma once

#include "IGraphicsApi.h"
#include "Vulkan/VulkanGraphicsApi.h"
#include "Architecture/Rollback/Rollback.h"
#include "Architecture/Ref.h"
#include "Entities/Level.h"

using namespace AVulkan;

class Renderer
{
public:
	//todo: use getter
	GLFWwindow* window;

	IGraphicsApi* GetGraphicsApi() { return graphicsApi; }

	Renderer(Ref<entt::registry> ecs, Rollback& engineRollback);
	~Renderer();

	void Init();
	void Run();

private: 
	Ref<entt::registry> ecs;
	IGraphicsApi* graphicsApi;

	Rollback* rollback;

	void InitGlfw();
	void SetupGlfwHints();
	void CreateAppWindow();

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

