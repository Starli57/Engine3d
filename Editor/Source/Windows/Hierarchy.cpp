
#include "Hierarchy.h"
#include "EngineShared/Components/NameComponent.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");

    auto view = ecs->registry->view<NameComponent>();

    for (int i = 0; i < ecs->allEntities.size(); i++)
    {
        auto entityRef = ecs->allEntities[i];
        std::string entityName = "Entity";

        if (entityRef->HasComponent<NameComponent>())
        {
            auto nameComponent = view.get<NameComponent>(entityRef->GetEntity());
            entityName = nameComponent.name;
        }

        if (selectedItemIndex == i) 
        {
            auto doSave = ImGui::Button("S");
            ImGui::SetItemTooltip("Save changes");

            ImGui::SameLine();
            auto doDelete = ImGui::Button("X");
            ImGui::SetItemTooltip("Delete entity");

            if (doSave)
            {
                ;
            }

            if (doDelete)
            {
                ;
            }

            ImGui::SameLine();
        }

        if (ImGui::Selectable(entityName.c_str(), selectedItemIndex == i ))
        {
            selectedItemIndex = i;
            inspector->Observe(ecs->allEntities[i]);
        }
    }

    if (ImGui::Button("Create entity"))
        ;

    ImGui::End();
}

Hierarchy::Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector) 
    : ecs(ecs), inspector(inspector)
{
}

Hierarchy::~Hierarchy()
{
}
