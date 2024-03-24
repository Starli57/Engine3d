#include "Pch.h"
#include "Camera.h"

Camera::Camera(Ref<entt::registry> ecs, float pov, float screenAspectRatio)
{
	this->ecs = ecs;
	this->pov = pov;

	UpdateScreenAspectRatio(screenAspectRatio);
}

Camera::~Camera()
{
}

void Camera::UpdateScreenAspectRatio(float screenAspectRatio)
{
	this->screenAspectRatio = screenAspectRatio;
}

void Camera::Update(float deltaTime = 0)
{
	auto entities = ecs->view<PositionComponent, UboViewProjectionComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& uboComponent = entities.get<UboViewProjectionComponent>(entity);

		uboComponent.view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		uboComponent.proj = glm::perspective(glm::radians(pov), screenAspectRatio, zNear, zFar);
		uboComponent.proj[1][1] *= -1;
	}
}