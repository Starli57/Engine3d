#include "CameraLookAtSystem.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Utilities/MathUtility.h"
#include "EngineCore/Utilities/PhysicsUtility.h"

CameraLookAtSystem::CameraLookAtSystem(const Ref<Ecs>& ecs, const Ref<Engine::InputManager>& input) : ecs(ecs), input(input)
{
}

void CameraLookAtSystem::Update(float deltaTime)
{
    float minDistance = FLT_MAX;
	
    const auto cameraEntities = ecs->registry->view<PositionComponent, RotationComponent, CameraComponent>();
    glm::vec3 cameraPosition;
    glm::vec3 cameraDirection;
    for (const auto entity : cameraEntities)
    {
        cameraPosition = cameraEntities.get<PositionComponent>(entity).position;
        auto& rotation = cameraEntities.get<RotationComponent>(entity).rotation;

        Engine::CalculateDirection(&cameraDirection, rotation);
        Engine::Normalize(&cameraDirection);
    }
	
    const auto selectableEntities = ecs->registry->view<PositionComponent, RotationComponent, ScaleComponent>();
    for (const auto entity : selectableEntities)
    {
        auto& position = selectableEntities.get<PositionComponent>(entity).position;
        auto& scale = selectableEntities.get<ScaleComponent>(entity).scale;
		
        float distance = FLT_MAX;
        Engine::DoesIntersectCube(cameraPosition, cameraDirection, position - scale  / 2.0f, position + scale  / 2.0f, distance);
        if (distance < minDistance) minDistance = distance;
    }

    if (minDistance < FLT_MAX) input->cameraLookAtPosition = Engine::GetIntersectionPoint(cameraPosition, cameraDirection, minDistance);
    else input->cameraLookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}
