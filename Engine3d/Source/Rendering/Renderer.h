#pragma once

#include "IRenderer.h"
#include "Vulkan/VulkanRenderer.h"
#include "Architecture/Rollback/Rollback.h"
#include "Objects/Level.h"

using namespace AVulkan;

class Renderer
{
public:
	GLFWwindow* window;

	Renderer(Rollback* engineRollback, Level* level);
	~Renderer();

	void Init();
	void Run();
	void AddMesh(Mesh& mesh);

private: 
	IRenderer* renderer;
	Level* level;

	Rollback* rollback;

	static void OnFramebufferResized(GLFWwindow* window, int width, int height);
};

