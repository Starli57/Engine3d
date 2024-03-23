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
		auto& uboComponent = transformComponents.get<UboModelComponent>(entity);
		auto& position = transformComponents.get<PositionComponent>(entity).position;
		auto& rotation = transformComponents.get<RotationComponent>(entity).rotation;
		auto& scale = transformComponents.get<ScaleComponent>(entity).scale;
		auto& uboModel = uboComponent.model;

		uboModel = glm::mat4(1.0f);
		uboModel = glm::translate(uboModel, position);
		uboModel = glm::rotate(uboModel, glm::radians(rotation.z), glm::vec3(0.0f, 1.0f, 0.0f));
		uboModel = glm::scale(uboModel, scale);

		uboComponent.model = uboModel;
	}
}
