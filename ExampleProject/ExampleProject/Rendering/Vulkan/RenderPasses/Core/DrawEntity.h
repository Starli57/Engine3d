#pragma once
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/UboModelComponent.h"

namespace VulkanApi
{
    struct DrawEntity
    {
        PositionComponent* positionComponent;
        UboModelComponent* uboModelComponent;
        MeshComponent* meshComponent; 
        MaterialComponent* materialComponent;
        
        DrawEntity() = default;
        
        DrawEntity(PositionComponent* positionComponent, UboModelComponent* uboModelComponent,
                MeshComponent* meshComponent, MaterialComponent* materialComponent)
            : positionComponent(positionComponent), uboModelComponent(uboModelComponent),
                meshComponent(meshComponent), materialComponent(materialComponent){}
    };

}
