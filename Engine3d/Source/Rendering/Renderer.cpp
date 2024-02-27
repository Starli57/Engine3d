#include "Pch.h"
#include "spdlog/spdlog.h"
#include "Renderer.h"

Renderer::Renderer(Ref<entt::registry> ecs, Rollback& mainRollback)
{
	this->ecs = ecs;
	this->rollback = new Rollback("Renderer", mainRollback);
}

Renderer::~Renderer()
{
	delete rollback;
}

void Renderer::Init()
{
	try
	{
		InitGlfw();
		SetupGlfwHints();
		CreateAppWindow();
		InitGraphicsApi();
	}
	catch (const std::exception& e)
	{
		spdlog::critical("Renderer critical error: {0}", e.what());
		rollback->Dispose();
		throw e;
	}
}

void Renderer::InitGlfw()
{
	auto status = glfwInit();
	CAssert::Check(status == GLFW_TRUE, "Glfw can't be init");
}

void Renderer::SetupGlfwHints()
{
#if GLFW_INCLUDE_VULKAN
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
}

void Renderer::CreateAppWindow()
{
	window = glfwCreateWindow(1000, 1000, "Engine window", nullptr, nullptr);
	CAssert::Check(window != nullptr, "GLFW window can't be created");

	rollback->Add([this]()
	{
		spdlog::info("Dispose glfw window");
		glfwDestroyWindow(window);
		glfwTerminate();
	});

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, OnFramebufferResized);
}

void Renderer::InitGraphicsApi()
{
#if GLFW_INCLUDE_VULKAN
	graphicsApi = new VulkanGraphicsApi(ecs, window, rollback);
	graphicsApi->Init();
	rollback->Add([this] { delete graphicsApi; });
#else
	throw std::runtime_error("Rendering api is not selected");
#endif
}

void Renderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//todo: handle exceptions and errors
		graphicsApi->Render();
	}
	graphicsApi->FinanilizeRenderOperations();
	spdlog::info("Window closed");
}

void Renderer::OnFramebufferResized(GLFWwindow* window, int width, int height)
{
	spdlog::debug("FramebufferResized");
	auto render = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	render->graphicsApi->OnFramebufferResized();
}