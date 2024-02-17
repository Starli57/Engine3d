#include "Pch.h"
#include "Renderer.h"
#include "spdlog/spdlog.h"

Renderer::Renderer(entt::registry& ecs, Rollback& mainRollback)
{
	this->ecs = &ecs;
	this->rollback = new Rollback(mainRollback);
}

Renderer::~Renderer()
{
	delete renderer;
	delete rollback;
}

void Renderer::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);//todo: replace to Vulkan specific logic

	try
	{
		window = glfwCreateWindow(1000, 1000, "Engine window", nullptr, nullptr);
		if (window == nullptr) throw std::runtime_error("glfw window can't be created");

		rollback->Add([this]()
		{
			spdlog::info("Dispose glfw window");
			glfwDestroyWindow(window);
			glfwTerminate();
		});

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, OnFramebufferResized);

		renderer = new VulkanRenderer(ecs, window, rollback);
		renderer->Init();
	}
	catch (const std::exception& e)
	{
		spdlog::critical("Renderer critical error: {0}", e.what());
		rollback->Dispose();
		throw e;
	}
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

void Renderer::AddMesh(Ref<Mesh> mesh)
{
	renderer->AddMesh(mesh);
}

void Renderer::OnFramebufferResized(GLFWwindow* window, int width, int height)
{
	spdlog::debug("FramebufferResized");
	auto render = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	render->renderer->OnFramebufferResized();
}