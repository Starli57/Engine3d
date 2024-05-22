#include "Inspector.h"

void Inspector::Update()
{
    auto show_another_window = true;
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window); 
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}
