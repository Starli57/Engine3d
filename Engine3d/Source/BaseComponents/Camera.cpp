#include "Pch.h"
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec4 rotation, float pov, float screenAspectRatio)
{
	transform = new Transform(position, rotation, glm::vec3(1, 1, 1));

	this->pov = pov;

	UpdateScreenAspectRatio(screenAspectRatio);
	UpdateUboViewProjection();
}

Camera::~Camera()
{
	delete transform;
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
