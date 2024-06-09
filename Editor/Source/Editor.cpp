#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Imgui/ImguiVulkan.h"
#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"
#include "Windows/ImguiDemo.h"
#include "Windows/VulkanDebugInfo.h"

Editor::Editor()
{
	projectSettings = CreateRef<ProjectSettigns>("../ExampleProject/");
	engine = CreateRef<Engine>(projectSettings);

	auto graphicsApi = engine->GetGraphicsApi();
	auto vulkanApi = static_cast<AVulkan::VulkanGraphicsApi*>(graphicsApi);

	editorUi = CreateRef<ImguiVulkan>(*vulkanApi);
	auto inspector = CreateRef<Inspector>();
	editorUi->AddWindow(inspector);
	editorUi->AddWindow(CreateRef<Hierarchy>(engine->GetEcs(), inspector));
	editorUi->AddWindow(CreateRef<ImguiDemo>());
	editorUi->AddWindow(CreateRef<VulkanDebugInfo>(*vulkanApi));

	engine->BindEditorUpdateFunction([this]() {editorUi->Update(); });
	engine->Run();
}

Editor::~Editor()
{
	//todo: destroy editor windows

	editorUi.reset();
	engine.reset();
}
