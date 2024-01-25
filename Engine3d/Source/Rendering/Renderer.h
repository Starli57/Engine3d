#pragma once

#include "IRenderer.h"
#include "Vulkan/VulkanRenderer.h"
#include "Architecture/Rollback/Rollback.h"

using namespace AVulkan;

class Renderer
{
public:
	GLFWwindow* window;

	Renderer(Rollback& engineRollback);
	~Renderer();

	void Init();
	void Run();

private: 
	IRenderer* renderer;

	Rollback* rollback;

	Mesh* mesh;

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

