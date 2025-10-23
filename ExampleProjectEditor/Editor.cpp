#include <memory>

#include "Editor.h"

#include "Editor/Imgui/ImguiVulkan.h"
#include "Editor/Windows/AssetsWindow.h"
#include "Editor/Windows/ImguiDemo.h"
#include "Editor/Windows/MainWindow.h"
#include "Editor/Windows/ProfilerWindow.h"
#include "Windows/Hierarchy.h"
#include "Windows/Inspector.h"
#include "Windows/SystemsStateWindow.h"
#include "Windows/VulkanTextureView.h"

namespace UserEditor
{
	Editor::Editor(const Ref<ProjectSettings>& projectSettings, const Ref<Engine::EngineApi>& engine)
	{
		const auto engineContext = engine->engineContext;
		const auto graphicsApi = engineContext->renderer;

		editorUi = CreateRef<::Editor::ImguiVulkan>(projectSettings, *graphicsApi);

		auto inspector = CreateRef<Inspector>(engineContext->resourcesStorage);
		editorUi->AddWindow(inspector);
		editorUi->AddWindow(CreateRef<::Editor::MainWindow>());
		editorUi->AddWindow(CreateRef<Hierarchy>(engineContext->entitySerializer, engineContext->ecs,
		                                                   inspector, projectSettings, engineContext->resourcesStorage));
		editorUi->AddWindow(CreateRef<::Editor::ImguiDemo>());
		editorUi->AddWindow(CreateRef<::Editor::ProfilerWindow>(engineContext));
		editorUi->AddWindow(CreateRef<::Editor::AssetsWindow>(engineContext->entitySerializer, engineContext->ecs, engineContext->resourcesStorage, projectSettings));
		editorUi->AddWindow(CreateRef<VulkanTextureView>(engineContext));
		editorUi->AddWindow(CreateRef<SystemsStateWindow>(engineContext->systemsState));
	
		engine->BindEditorUpdateFunction([this]() { editorUi->Update(); });
	}

	Editor::~Editor()
	{
		//todo: destroy editor windows

		editorUi.reset();
	}
}
