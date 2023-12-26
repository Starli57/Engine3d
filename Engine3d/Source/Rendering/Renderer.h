#pragma once

#include "Vulkan/VulkanRenderer.h"

class Renderer
{
public:
	GLFWwindow* window;

	Renderer();
	~Renderer();

	void Run();

private: 
	VulkanRenderer* vkRenderer;
};

