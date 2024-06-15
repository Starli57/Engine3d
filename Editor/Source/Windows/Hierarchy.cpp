
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

        if (ImGui::Selectable(entityName.c_str(), selectedItemIndex == i))
        {
            selectedItemIndex = i;
            inspector->Observe(ecs->allEntities[i]);
        }
    }

    ImGui::End();
}

Hierarchy::Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector) 
    : ecs(ecs), inspector(inspector), selectedItemIndex(-1)
{
}

Hierarchy::~Hierarchy()
{
}
