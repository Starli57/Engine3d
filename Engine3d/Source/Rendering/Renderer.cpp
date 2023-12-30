#include "Pch.h"
#include "Renderer.h"

Renderer::Renderer()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);//todo: replace to Vulkan specific logic
	window = glfwCreateWindow(1000, 1000, "Engine window", nullptr, nullptr);

	vkRenderer = new VulkanRenderer();//todo: replace to Vulkan specific logic
}

Renderer::~Renderer()
{
	delete vkRenderer;

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	std::cout << "Window closed" << std::endl;
}
