#include "EngineCore/Pch.h"
#include "Camera.h"

Camera::Camera(Ref<Ecs> ecs, GLFWwindow* window)
{
	this->ecs = ecs;
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

	auto entities = ecs->registry->view<PositionComponent, RotationComponent, UboViewProjectionComponent, CameraComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& rotation = entities.get<RotationComponent>(entity).rotation;
		auto& uboComponent = entities.get<UboViewProjectionComponent>(entity);
		auto& cameraComponent = entities.get<CameraComponent>(entity);

		glm::vec3 direction;
		direction.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		direction.y = sin(glm::radians(rotation.y));
		direction.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		auto viewPoint = glm::normalize(direction);

		uboComponent.view = glm::lookAt(position, position + viewPoint, cameraComponent.upAxis);
		uboComponent.proj = glm::perspective(glm::radians(cameraComponent.fov), 
			screenAspectRatio, cameraComponent.zNear, cameraComponent.zFar);
		uboComponent.proj[1][1] *= -1;
	}
} 