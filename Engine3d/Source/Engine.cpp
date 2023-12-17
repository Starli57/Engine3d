#include "Pch.h"
#include "Engine.h"
#include <GLFW/glfw3.h>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::Run()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(500, 500, "Engine window", nullptr, nullptr);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		std::cout << "Engine Run";
	}

	glfwDestroyWindow(window);
	std::cout << "Window closed";
}
