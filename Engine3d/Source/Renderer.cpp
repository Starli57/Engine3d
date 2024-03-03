#include "Pch.h"
#include "Components/MeshContainer.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
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
		SubscribeGraphicsApiEvents();
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
	graphicsApi = new VulkanGraphicsApi(window, rollback);
	graphicsApi->Init();
	rollback->Add([this] { delete graphicsApi; });
#else
	throw std::runtime_error("Rendering api is not selected");
#endif
}

void Renderer::SubscribeGraphicsApiEvents()
{
	graphicsApi->OnFrameBufferAspectRatioChanged.AddHandler([this](float aspectRation)
		{
			auto cameraEntities = ecs->view<Camera>();
			for (auto entity : cameraEntities)
			{
				auto camera = cameraEntities.get<Camera>(entity);
				camera.UpdateScreenAspectRatio(aspectRation);
				camera.UpdateUbo();
			}
		});
	//todo: remove the handler in rollback
}

void Renderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		auto meshContainers = ecs->view<Transform, MeshContainer>();
		std::vector<RenderModel> renderModels;
		//renderModels.reserve(meshContainers.size_hint)//todo: can reserve?

		for (auto entity : meshContainers)
		{
			auto [transform, meshConatiner] = meshContainers.get<Transform, MeshContainer>(entity);
			auto uboModel = transform.GetUboModel();
			auto mesh = meshConatiner.GetMesh();
			renderModels.push_back(RenderModel(uboModel, mesh));
		}

		//todo: find most relevant camera
		auto cameraEntities = ecs->view<Camera>();
		auto [mainCamera] = cameraEntities.get(cameraEntities.front());
		auto ubo = mainCamera.GetUbo();

		//todo: handle exceptions and errors
		graphicsApi->Render(renderModels, ubo);
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