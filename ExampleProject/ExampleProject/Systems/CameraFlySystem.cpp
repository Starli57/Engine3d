#include "CameraFlySystem.h"

#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Profiler/Profiler.h"
#include "ExampleProject/Components/CameraFreeComponent.h"

CameraFlySystem::CameraFlySystem(const Ref<Ecs>& ecs, const Ref<InputManager>& input)
	: ecs(ecs), input(input)
{
}

void CameraFlySystem::Update(const float deltaTime)
{
	Profiler::GetInstance().BeginSample("CameraFlySystem Update");
	const auto entities = ecs->registry->view<PositionComponent, RotationComponent, CameraFreeComponent>();
	for (const auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& rotation = entities.get<RotationComponent>(entity).rotation;
		const auto& freeCamComponent = entities.get<CameraFreeComponent>(entity);
		
		if (input->IsKeyPressed(GLFW_KEY_LEFT_ALT))
		{
			double deltaRotationX;
			double deltaRotationY;
			input->GetCursorDelta(deltaRotationX, deltaRotationY);

			rotation.x += static_cast<float>(deltaRotationX) * freeCamComponent.rotationSpeed * deltaTime;
			rotation.y += static_cast<float>(-deltaRotationY) * freeCamComponent.rotationSpeed * deltaTime;
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
	Profiler::GetInstance().EndSample();
}
