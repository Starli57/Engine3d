#include "Inspector.h"
#include "ComponentInspector.h"

void Inspector::Update()
{
    if (observingEntity.get() == nullptr) return;

    ImGui::Begin("Inspector");

    NameInspector().Update(observingEntity);
    PositionInspector().Update(observingEntity);
    RotationInspector().Update(observingEntity);
    RotationVelocityInspector().Update(observingEntity);
    ScaleInspector().Update(observingEntity);

    CameraInspector().Update(observingEntity);

    DiffuseLightInspector().Update(observingEntity);

    ImGui::End();
}

void Inspector::Observe(Ref<Entity> entity)
{
    observingEntity = entity;
}
