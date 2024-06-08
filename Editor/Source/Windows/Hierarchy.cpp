
#include "Hierarchy.h"
#include "SharedLib/Components/NameComponent.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");

    auto nameFilter = ecs->view<NameComponent>();
    for (int i = 0; i < nameFilter.size(); i++)
    {
        auto nameComponent = nameFilter.get<NameComponent>(nameFilter[i]);
        if (ImGui::Selectable(nameComponent.name.c_str(), selectedItemIndex == i))
            selectedItemIndex = i;
    }

    ImGui::End();
}

Hierarchy::Hierarchy(Ref<entt::registry> ecs) : ecs(ecs)
{
}

Hierarchy::~Hierarchy()
{
}
