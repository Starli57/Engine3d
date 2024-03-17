#include "Editor.h"

Editor::Editor()
{
    RunImgui([this]() {Run(); });
}

Editor::~Editor()
{
}

void Editor::Run()
{
    ImGui::Begin("Another Window");
    ImGui::Text("Hello from another window!");
    ImGui::End();
}