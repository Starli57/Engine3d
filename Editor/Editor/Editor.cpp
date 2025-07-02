#include <iostream>
#include <thread>
#include <memory>

#include "Editor.h"

#include "Imgui/ImguiVulkan.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"
#include "Windows/ImguiDemo.h"
#include "Windows/AssetsWindow.h"
#include "Windows/ProfilerWindow.h"
#include "Windows/VulkanTextureView.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "Windows/BuildingsMenu.h"
#include "Windows/MainWindow.h"
#include "Windows/SystemsStateWindow.h"

Editor::Editor(const Ref<ProjectSettings>& projectSettings, const Ref<Engine>& engine) : 
	projectSettings(projectSettings), engine(engine)
{
	const auto graphicsApi = engine->GetGraphicsApi();

	editorUi = CreateRef<ImguiVulkan>(projectSettings, *graphicsApi);

	auto inspector = CreateRef<Inspector>(engine->GetAssetsDatabase());
	editorUi->AddWindow(inspector);
	editorUi->AddWindow(CreateRef<MainWindow>());
	editorUi->AddWindow(CreateRef<Hierarchy>(engine->GetSerializer(), engine->GetEcs(), inspector, projectSettings, engine->GetAssetsDatabase()));
	editorUi->AddWindow(CreateRef<ImguiDemo>());
	editorUi->AddWindow(CreateRef<ProfilerWindow>(engine, *graphicsApi));
	editorUi->AddWindow(CreateRef<VulkanTextureView>(engine, *graphicsApi));
	editorUi->AddWindow(CreateRef<AssetsWindow>(engine->GetSerializer(), engine->GetEcs(), engine->GetAssetsDatabase(), projectSettings));
	editorUi->AddWindow(CreateRef<SystemsStateWindow>(engine->GetSystemsState()));
	editorUi->AddWindow(CreateRef<BuildingsMenu>());
	
	engine->BindEditorUpdateFunction([this]() { editorUi->Update(); });
}

Editor::~Editor()
{
	//todo: destroy editor windows

	editorUi.reset();
}
