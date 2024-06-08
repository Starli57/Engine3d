
#include "Hierarchy.h"
#include "EngineShared/Components/NameComponent.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");

    auto nameFilter = ecs->registry->view<NameComponent>();
    for (int i = 0; i < nameFilter.size(); i++)
    {
        auto nameComponent = nameFilter.get<NameComponent>(nameFilter.begin()[i]);
        if (ImGui::Selectable(nameComponent.name.c_str(), selectedItemIndex == i))
            selectedItemIndex = i;
    }

    ImGui::End();
}

Hierarchy::Hierarchy(Ref<Ecs> ecs) : ecs(ecs), selectedItemIndex(0)
{
}

Hierarchy::~Hierarchy()
{
}
