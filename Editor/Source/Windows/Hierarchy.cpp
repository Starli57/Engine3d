
#include "Hierarchy.h"
#include "SharedLib/Components/NameComponent.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");
    auto nameFilter = ecs->view<NameComponent>();
    for (auto entity : nameFilter)
    {
        auto nameComponent = nameFilter.get<NameComponent>(entity);
        ImGui::Text(nameComponent.name.c_str());
    }
    ImGui::End();
}

Hierarchy::Hierarchy(Ref<entt::registry> ecs) : ecs(ecs)
{
}

Hierarchy::~Hierarchy()
{
}
