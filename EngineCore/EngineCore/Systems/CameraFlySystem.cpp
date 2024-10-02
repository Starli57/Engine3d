#pragma once

#include "EngineCore/Pch.h"
#include "CameraFlySystem.h"

CameraFlySystem::CameraFlySystem(Ref<Ecs> ecs, Ref<Input> input)
	: ecs(ecs), input(input)
{
}

CameraFlySystem::~CameraFlySystem()
{
}

void CameraFlySystem::Update(float deltaTime)
{
	auto entities = ecs->registry->view<PositionComponent, RotationComponent, CameraFreeComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& rotation = entities.get<RotationComponent>(entity).rotation;
		auto& freeCamComponent = entities.get<CameraFreeComponent>(entity);
		
		if (input->IsKeyPressed(GLFW_KEY_LEFT_ALT))
		{
			double deltaRotationX;
			double deltaRotationY;
			input->GetCursorDelta(deltaRotationX, deltaRotationY);

			rotation.x += (float)deltaRotationX * freeCamComponent.rotationSpeed * deltaTime;
			rotation.y += (float)-deltaRotationY * freeCamComponent.rotationSpeed * deltaTime;
		}

		glm::vec3 direction;
		direction.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		direction.y = sin(glm::radians(rotation.y));
		direction.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));

		auto forward = glm::normalize(direction);
		auto up = glm::vec3(0, 1, 0);
		auto right = glm::normalize(glm::cross(forward, up));

		if (input->IsKeyPressed(GLFW_KEY_D))
			position += right * freeCamComponent.movementSpeed * deltaTime;
		if (input->IsKeyPressed(GLFW_KEY_A))
			position -= right * freeCamComponent.movementSpeed * deltaTime;
		if (input->IsKeyPressed(GLFW_KEY_W))
			position += forward * freeCamComponent.movementSpeed * deltaTime;
		if (input->IsKeyPressed(GLFW_KEY_S))
			position -= forward * freeCamComponent.movementSpeed * deltaTime;
	}
}
