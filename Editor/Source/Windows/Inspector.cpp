#include "Inspector.h"
#include "ComponentInspector.h"
#include "EngineShared/ComponentsUtility.h"

void Inspector::Update()
{
    if (observingEntity.get() == nullptr) return;

    ImGui::Begin("Inspector");

    auto componentInspector = CreateUniqueRef<ComponentInspector>();
    componentInspector->Update(observingEntity);

    ShowAddComponentBtn();

    ImGui::End();
}

void Inspector::Observe(Ref<Entity> entity)
{
    observingEntity = entity;
}

void Inspector::ShowAddComponentBtn()
{
    static int addComponentSelectdIndex = -1;

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Add component.."))
        ImGui::OpenPopup("AddComponentPopup");

    if (ImGui::BeginPopup("AddComponentPopup"))
    {
        for (int i = 0; i < componentsLength; i++)
        {
            if (ImGui::Selectable(componentsNames[i]))
            {
                addComponentSelectdIndex = i;
                AddComponent(observingEntity, componentsNames[i]);
            }
        }

        ImGui::EndPopup();
    }
}
