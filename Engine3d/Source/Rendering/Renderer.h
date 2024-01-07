#pragma once

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
	VulkanRenderer* vkRenderer;
	Rollback* vulkanRollback;
};

