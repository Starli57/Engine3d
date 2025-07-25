#include "SystemsStateWindow.h"
#include "imgui.h"


/// <summary>
/// This script is autogenerated
/// Don't make any changes in the file
/// </summary>
SystemsStateWindow::SystemsStateWindow(const Ref<SystemsState>& systemsState) : systemsState(systemsState) { } 

void SystemsStateWindow::Update()
{
      ImGui::Begin("Systems State");
      ImGui::Checkbox("CameraFlySystem", &systemsState->CameraFlySystem);
      ImGui::Checkbox("CameraLookAtSystem", &systemsState->CameraLookAtSystem);
      ImGui::Checkbox("RotateSystem", &systemsState->RotateSystem);
      ImGui::Checkbox("TransformSystem", &systemsState->TransformSystem);
      ImGui::Checkbox("ViewProjectionSystem", &systemsState->ViewProjectionSystem);
      ImGui::End();
};
