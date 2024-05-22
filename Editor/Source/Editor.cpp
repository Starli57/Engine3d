#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"

Editor::Editor()
{
	projectSettings = CreateRef<ProjectSettigns>("../ExampleProject/");

	engine = CreateRef<Engine>(projectSettings);

	auto graphicsApi = engine->GetGraphicsApi();
	auto vulkanApi = static_cast<AVulkan::VulkanGraphicsApi*>(graphicsApi);

	imgui = CreateRef<ImguiVulkan>(*vulkanApi);
	imgui->AddWindow(CreateRef<Hierarchy>());
	imgui->AddWindow(CreateRef<Inspector>());

	engine->BindEditor(imgui);


	engine->Run();

}

Editor::~Editor()
{
	//todo: destroy editor windows

	imgui.reset();
	engine.reset();
}
