#include "Pch.h"
#include "Renderer.h"

Renderer::Renderer()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);//todo: replace to Vulkan specific logic

	try
	{
		window = glfwCreateWindow(1000, 1000, "Engine window", nullptr, nullptr);
		if (window == nullptr) throw std::runtime_error("glfw window can't be created");

		//todo: replace to Vulkan specific logic
		vulkanRollback = new Rollback();
		vkRenderer = new VulkanRenderer(window, vulkanRollback);
		vkRenderer->Initialize();
	}
	catch (const std::exception& e)
	{
		//todo: add errors handling
	}
}

Renderer::~Renderer()
{
	delete vkRenderer;
	delete vulkanRollback;

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	spdlog::info("Window closed");
}
