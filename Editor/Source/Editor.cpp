#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Rendering/Vulkan/VulkanGraphicsApi.h"

Editor::Editor()
{
	projectSettings = CreateRef<ProjectSettigns>("../ExampleProject/");

	engine = CreateRef<Engine>(projectSettings);

	auto graphicsApi = engine->GetGraphicsApi();
	auto vulkanApi = static_cast<AVulkan::VulkanGraphicsApi*>(graphicsApi);
	imgui = CreateRef<ImguiVulkan>(*vulkanApi);
	engine->BindEditor(imgui);

	engine->Run();

}

Editor::~Editor()
{
	imgui.reset();
	engine.reset();
}
