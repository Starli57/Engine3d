#include <iostream>
#include <thread>

#include "Editor.h"

Editor::Editor()
{
	projectSettings = CreateRef<ProjectSettigns>(
		"../ExampleProject/"
	);

	engine = CreateRef<Engine>(projectSettings);
	std::thread engineThread([this]() {engine->Run(); });
	std::thread editorUiThread([this]() {RunImgui([this]() { UpdateEditor(); }); });

	engineThread.join();
	editorUiThread.join();
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
