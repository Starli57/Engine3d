#pragma once

#include "IRenderer.h"
#include "Vulkan/VulkanRenderer.h"
#include "Architecture/Rollback/Rollback.h"
#include "Entities/Level.h"
#include "Architecture/Ref.h"

using namespace AVulkan;

//todo: no reason to have separated Renderer and specific Renderer classes, need to remove additional layer
class Renderer
{
public:
	//todo: use getter
	GLFWwindow* window;

	IRenderer* GetSpecificRenderer() { return renderer; }

	Renderer(Ref<entt::registry> ecs, Rollback& engineRollback);
	~Renderer();

	void Init();
	void Run();

private: 
	Ref<entt::registry> ecs;
	IRenderer* renderer;

	Rollback* rollback;

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

