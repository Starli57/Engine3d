#include "Inspector.h"
#include "ComponentInspector.h"

void Inspector::Update()
{
    if (observingEntity.get() == nullptr) return;

    ImGui::Begin("Inspector");

    auto componentInspector = CreateUniqueRef<ComponentInspector>();
    componentInspector->Update(observingEntity);

    ImGui::End();
}

void Inspector::Observe(Ref<Entity> entity)
{
    observingEntity = entity;
}
