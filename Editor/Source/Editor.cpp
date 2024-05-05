#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Rendering/Vulkan/VulkanGraphicsApi.h"

Editor::Editor()
{
	projectSettings = CreateRef<ProjectSettigns>(
		"../ExampleProject/"
	);

	engine = CreateRef<Engine>(projectSettings);
	std::thread engineThread([this]() {engine->Run(); });

	auto graphicsApi = engine->GetGraphicsApi();
	auto vulkanApi = static_cast<AVulkan::VulkanGraphicsApi*>(graphicsApi);

	imgui = CreateRef<ImguiVulkan>();
	imgui->RunImgui(vulkanApi);//[this]() { UpdateEditor(); });
}

Editor::~Editor()
{
}


void Editor::UpdateEngine()
{
}

void Editor::UpdateEditor()
{
    ImGui::Begin("Another Window");
    ImGui::Text("Hello from another window!");
    ImGui::End();
}
