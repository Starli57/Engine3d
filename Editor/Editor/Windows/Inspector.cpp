#include "Inspector.h"
#include "EngineCore/ComponentsUtility.h"

Inspector::Inspector(Ref<AssetsDatabase> assetsDatabase) : assetsDatabase(assetsDatabase)
{
    componentsRenderer = CreateUniqueRef<ComponentsRenderer>(assetsDatabase);
}

void Inspector::Update()
{
    if (observingEntity.get() == nullptr) return;

    ImGui::Begin("Inspector");

    componentsRenderer->Update(observingEntity);

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
    static int removeComponentSelectdIndex = -1;

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

    ImGui::SameLine();

    if (ImGui::Button("Remove component.."))
        ImGui::OpenPopup("RemoveComponentPopup");

    if (ImGui::BeginPopup("RemoveComponentPopup"))
    {
        for (int i = 0; i < componentsLength; i++)
        {
            if (ImGui::Selectable(componentsNames[i]))
            {
                removeComponentSelectdIndex = i;
                RemoveComponent(observingEntity, componentsNames[i]);
            }
        }

        ImGui::EndPopup();
    }
}
