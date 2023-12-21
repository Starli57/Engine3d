#include "Pch.h"
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::Run()
{
	glm::vec3 v3 = glm::vec3(1.0f, 0.1f, 0.0f);
	v3.r *= 2;

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
	std::cout << "V3 = " << v3.r << v3.g << v3.b;
}
