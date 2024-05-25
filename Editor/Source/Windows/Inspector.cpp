#include "Inspector.h"

void Inspector::Update()
{
    ImGui::Begin("Another Window");
    ImGui::Text("Hello from another window!");
    ImGui::End();
}