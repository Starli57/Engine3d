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

	auto entities = ecs->registry->view<PositionComponent, UboViewProjectionComponent, CameraComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& uboComponent = entities.get<UboViewProjectionComponent>(entity);
		auto& cameraComponent = entities.get<CameraComponent>(entity);

		uboComponent.view = glm::lookAt(position, cameraComponent.lookPoint, cameraComponent.upAxis);
		uboComponent.proj = glm::perspective(glm::radians(cameraComponent.fov), 
			screenAspectRatio, cameraComponent.zNear, cameraComponent.zFar);
		uboComponent.proj[1][1] *= -1;
	}
} 