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
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#include "Windows/MainWindow.h"
#include "Windows/ResourcesConverterWindow.h"
#include "Windows/SystemsStateWindow.h"

Editor::Editor(const Ref<ProjectSettings>& projectSettings, const Ref<Engine::EngineApi>& engine)
{
	const auto engineContext = engine->engineContext;
	const auto graphicsApi = engineContext->renderer;

	editorUi = CreateRef<ImguiVulkan>(projectSettings, *graphicsApi);

	auto inspector = CreateRef<Inspector>(engineContext->resourcesStorage);
	editorUi->AddWindow(inspector);
	editorUi->AddWindow(CreateRef<MainWindow>());
	editorUi->AddWindow(CreateRef<Hierarchy>(engineContext->entitySerializer, engineContext->ecs,
		inspector, projectSettings, engineContext->resourcesStorage));
	editorUi->AddWindow(CreateRef<ImguiDemo>());
	editorUi->AddWindow(CreateRef<ProfilerWindow>(engineContext));
	editorUi->AddWindow(CreateRef<VulkanTextureView>(engineContext));
	editorUi->AddWindow(CreateRef<AssetsWindow>(engineContext->entitySerializer, engineContext->ecs, engineContext->resourcesStorage, projectSettings));
	editorUi->AddWindow(CreateRef<SystemsStateWindow>(engineContext->systemsState));
	
	engine->BindEditorUpdateFunction([this]() { editorUi->Update(); });
}

Editor::~Editor()
{
	//todo: destroy editor windows

	editorUi.reset();
}
