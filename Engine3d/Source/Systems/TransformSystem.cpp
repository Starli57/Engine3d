#include "Pch.h"
#include "TransformSystem.h"

TransformSystem::TransformSystem(Ref<entt::registry> ecs) : ecs(ecs)
{
}

void TransformSystem::Update()
{
	auto transformComponents = ecs->view<UboModelComponent, PositionComponent, RotationComponent, ScaleComponent>();
	for (auto entity : transformComponents)
	{
		auto uboComponent = transformComponents.get<UboModelComponent>(entity);
		auto positionComponent = transformComponents.get<PositionComponent>(entity);
		auto rotationComponent = transformComponents.get<RotationComponent>(entity);
		auto scaleComponent = transformComponents.get<ScaleComponent>(entity);

		auto position = positionComponent.position;
		auto rotation = rotationComponent.rotation;
		auto scale = scaleComponent.scale;

		auto uboModel = uboComponent.model;
		uboModel = glm::mat4(1.0f);
		uboModel = glm::translate(uboModel, position);
		//todo: add rotation	uboModel = glm::rotate(uboModel, 1, rotation);
		uboModel = glm::scale(uboModel, scale);
		uboComponent.model = uboModel;
	}
}
