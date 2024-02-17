#pragma once

#include "IRenderer.h"
#include "Vulkan/VulkanRenderer.h"
#include "Architecture/Rollback/Rollback.h"
#include "Entities/Level.h"
#include "Architecture/Ref.h"

using namespace AVulkan;

class Renderer
{
public:
	GLFWwindow* window;

	Renderer(Ref<entt::registry> ecs, Rollback& engineRollback);
	~Renderer();

	void Init();
	void Run();
	void AddMesh(Ref<Mesh> mesh);

private: 
	Ref<entt::registry> ecs;
	IRenderer* renderer;

	Rollback* rollback;

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

