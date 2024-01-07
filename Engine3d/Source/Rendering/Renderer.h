#pragma once

#include "Vulkan/VulkanRenderer.h"
#include "../Architecture/Rollback/Rollback.h"

class Renderer
{
public:
	GLFWwindow* window;

	Renderer();
	~Renderer();

	void Run();

private: 
	VulkanRenderer* vkRenderer;
	Rollback* vulkanRollback;
};

