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
	void AddMesh(Mesh& mesh);

private: 
	IRenderer* renderer;

	Rollback* rollback;

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

