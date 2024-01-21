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
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, OnFramebufferResized);

		rendererRollback = new Rollback();

		renderer = new VulkanRenderer(window, rendererRollback);
		renderer->Initialize();
	}
	catch (const std::exception& e)
	{
		//todo: add errors handling
	}
}

Renderer::~Renderer()
{
	delete renderer;
	delete rendererRollback;

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//todo: handle exceptions and errors
		renderer->Render();
	}
	renderer->FinanilizeRenderOperations(); 
	spdlog::info("Window closed");
}

void Renderer::OnFramebufferResized(GLFWwindow* window, int width, int height)
{
	spdlog::debug("FramebufferResized");
	auto render = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	render->renderer->OnFramebufferResized();
}