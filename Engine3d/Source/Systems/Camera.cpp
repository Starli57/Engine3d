#include "Pch.h"
#include "Camera.h"

Camera::Camera(Ref<Entity> entity, float pov, float screenAspectRatio)
{
	this->entity = entity;
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

void Camera::Update()
{
	auto position = entity->GetComponent<PositionComponent>().position;
	auto uboComponent = entity->GetComponent<UboViewProjectionComponent>();

	uboComponent.view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	uboComponent.proj = glm::perspective(glm::radians(pov), screenAspectRatio, zNear, zFar);
	uboComponent.proj[1][1] *= -1;
}