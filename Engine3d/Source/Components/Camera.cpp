#include "Pch.h"
#include "Camera.h"

Camera::Camera(Ref<Transform> transform, float pov, float screenAspectRatio)
{
	this->transform = transform;
	this->pov = pov;

	UpdateScreenAspectRatio(screenAspectRatio);
	UpdateUboViewProjection();
}

Camera::~Camera()
{
}

void Camera::UpdateScreenAspectRatio(float screenAspectRatio)
{
	this->screenAspectRatio = screenAspectRatio;
}

void Camera::UpdateUboViewProjection()
{
	uboViewProjection.view = glm::lookAt(transform->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	uboViewProjection.proj = glm::perspective(glm::radians(pov), screenAspectRatio, zNear, zFar);
	uboViewProjection.proj[1][1] *= -1;
}

UboViewProjection Camera::GetUboViewProjection()
{
	return uboViewProjection;
}
