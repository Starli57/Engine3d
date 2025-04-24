#pragma once
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/UboModelComponent.h"


namespace AVulkan
{
    struct DrawEntity
    {
        PositionComponent* positionComponent;
        UboModelComponent* uboModelComponent;
        MeshComponent* meshComponent; 
        MaterialComponent* materialComponent;

        DrawEntity(PositionComponent* positionComponent, UboModelComponent* uboModelComponent, MeshComponent* meshComponent, MaterialComponent* materialComponent)
            : positionComponent(positionComponent), uboModelComponent(uboModelComponent), meshComponent(meshComponent), materialComponent(materialComponent){}
    };
}
