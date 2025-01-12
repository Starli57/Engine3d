#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Imgui/ImguiVulkan.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"
#include "Windows/ImguiDemo.h"
#include "Windows/AssetsWindow.h"
#include "Windows/VulkanDebugInfo.h"
#include "Windows/VulkanTextureView.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"

Editor::Editor(Ref<ProjectSettings> projectSettings, Ref<Engine> engine) : 
	projectSettings(projectSettings), engine(engine)
{
	auto graphicsApi = engine->GetGraphicsApi();
	auto vulkanApi = static_cast<AVulkan::GraphicsApiVulkan*>(graphicsApi);

	editorUi = CreateRef<ImguiVulkan>(projectSettings, *vulkanApi);

	auto inspector = CreateRef<Inspector>(engine->GetAssetsDatabase());
	editorUi->AddWindow(inspector);
	editorUi->AddWindow(CreateRef<Hierarchy>(engine->GetEcs(), inspector, projectSettings, engine->GetAssetsDatabase()));
	editorUi->AddWindow(CreateRef<ImguiDemo>());
	editorUi->AddWindow(CreateRef<VulkanDebugInfo>(engine, *vulkanApi));
	editorUi->AddWindow(CreateRef<VulkanTextureView>(engine, *vulkanApi));
	editorUi->AddWindow(CreateRef<AssetsWindow>(engine->GetEcs(), engine->GetAssetsDatabase(), projectSettings));

	engine->BindEditorUpdateFunction([this]() { editorUi->Update(); });
}

Editor::~Editor()
{
	//todo: destroy editor windows

	editorUi.reset();
	engine.reset();
}
