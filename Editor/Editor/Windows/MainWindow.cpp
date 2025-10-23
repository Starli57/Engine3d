#include "MainWindow.h"

#include "imgui.h"

namespace Editor
{
    void MainWindow::Update()
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_MenuBar;
    
        ImGui::Begin("Editor", nullptr, flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Test"))
            {
                ImGui::SeparatorText("Test1");
                ImGui::MenuItem("Test2", nullptr, nullptr);
                ImGui::MenuItem("Test3", nullptr, nullptr);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
}