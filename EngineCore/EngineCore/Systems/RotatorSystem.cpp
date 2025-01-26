#include "EngineCore/Pch.h"
#include "RotatorSystem.h"
#include "EngineCore/Components/RotationVelocityComponent.h"

RotatorSystem::RotatorSystem(const Ref<Ecs>& ecs) : ecs(ecs)
{
}

void RotatorSystem::Update(const float deltaTime)
{
	const auto rotationComponents = ecs->registry->view<RotationVelocityComponent, RotationComponent>();
	for (const auto entity : rotationComponents)
	{
		auto& rotationComponent = rotationComponents.get<RotationComponent>(entity);
		auto& rotationVelocityComponent = rotationComponents.get<RotationVelocityComponent>(entity);

		rotationComponent.rotation += rotationVelocityComponent.velocity * deltaTime;
	}
}
