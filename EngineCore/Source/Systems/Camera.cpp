#include "Pch.h"
#include "Camera.h"

Camera::Camera(Ref<Ecs> ecs, GLFWwindow* window, float pov)
{
	this->ecs = ecs;
	this->pov = pov;
	this->window = window;
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime = 0)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	if (width == 0 || height == 0) return;

	auto screenAspectRatio = width / (float)height;

	auto entities = ecs->registry->view<PositionComponent, UboViewProjectionComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& uboComponent = entities.get<UboViewProjectionComponent>(entity);

		uboComponent.view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		uboComponent.proj = glm::perspective(glm::radians(pov), screenAspectRatio, zNear, zFar);
		uboComponent.proj[1][1] *= -1;
	}
} 