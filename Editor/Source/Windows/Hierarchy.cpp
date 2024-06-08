
#include "Hierarchy.h"
#include "EngineShared/Components/NameComponent.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");

    auto view = ecs->registry->view<NameComponent>();

    for (int i = 0; i < ecs->allEntities.size(); i++)
    {
        auto nameComponent = view.get<NameComponent>(ecs->allEntities[i]->GetEntity());
        if (ImGui::Selectable(nameComponent.name.c_str(), selectedItemIndex == i))
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
