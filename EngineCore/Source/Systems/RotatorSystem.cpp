#include "Pch.h"
#include "RotatorSystem.h"
#include "EngineShared/Components/RotationVelocityComponent.h"

RotatorSystem::RotatorSystem(Ref<entt::registry> ecs) : ecs(ecs)
{
}

void RotatorSystem::Update(float deltaTime)
{
	auto rotationComponents = ecs->view<RotationVelocityComponent, RotationComponent>();
	for (auto entity : rotationComponents)
	{
		auto& rotationComponent = rotationComponents.get<RotationComponent>(entity);
		auto& rotationVelocityComponent = rotationComponents.get<RotationVelocityComponent>(entity);

		rotationComponent.rotation += rotationVelocityComponent.velocity * deltaTime;
	}
}
