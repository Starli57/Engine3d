#pragma once

#include "IRenderer.h"
#include "Vulkan/VulkanRenderer.h"
#include "Architecture/Rollback/Rollback.h"

using namespace AVulkan;

class Renderer
{
public:
	GLFWwindow* window;

	Renderer();
	~Renderer();

	void Run();

private: 
	IRenderer* renderer;
	Rollback* rendererRollback;
};

