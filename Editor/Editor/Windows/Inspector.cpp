#include "Inspector.h"
#include "EngineCore/Utilities/ComponentsUtility.h"

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

void Inspector::Observe(const Ref<Entity>& entity)
{
    observingEntity = entity;
}

void Inspector::ShowAddComponentBtn() const
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Add component.."))
        ImGui::OpenPopup("AddComponentPopup");

    if (ImGui::BeginPopup("AddComponentPopup"))
    {
        for (int i = 0; i < componentsLength; i++)
        {
            if (ImGui::Selectable(allComponentsNames[i]))
            {
                AddComponent(observingEntity, allComponentsNames[i]);
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
            if (ImGui::Selectable(allComponentsNames[i]))
            {
                RemoveComponent(observingEntity, allComponentsNames[i]);
            }
        }

        ImGui::EndPopup();
    }
}
