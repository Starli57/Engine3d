#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Imgui/ImguiVulkan.h"
#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"
#include "Windows/ImguiDemo.h"

Editor::Editor()
{
	projectSettings = CreateRef<ProjectSettigns>("../ExampleProject/");
	engine = CreateRef<Engine>(projectSettings);

	auto graphicsApi = engine->GetGraphicsApi();
	auto vulkanApi = static_cast<AVulkan::VulkanGraphicsApi*>(graphicsApi);

	editorUi = CreateRef<ImguiVulkan>(*vulkanApi);
	editorUi->AddWindow(CreateRef<Hierarchy>(engine->GetEcs()));
	editorUi->AddWindow(CreateRef<Inspector>());
	editorUi->AddWindow(CreateRef<ImguiDemo>());

	engine->BindEditor(editorUi);
	engine->Run();
}

Editor::~Editor()
{
	//todo: destroy editor windows

	editorUi.reset();
	engine.reset();
}
