#include "EngineCore/Pch.h"
#include "ViewProjectionSystem.h"

ViewProjectionSystem::ViewProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window)
{
	this->ecs = ecs;
	this->window = window;
}

void ViewProjectionSystem::Update(float deltaTime = 0)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	if (width == 0 || height == 0) return;

	const auto screenAspectRatio = static_cast<float>(width) / static_cast<float>(height);

	const auto cameraEntities = ecs->registry->view<PositionComponent, RotationComponent, UboViewProjectionComponent, CameraComponent>();
	for (const auto entity : cameraEntities)
	{
		auto& position = cameraEntities.get<PositionComponent>(entity).position;
		auto& rotation = cameraEntities.get<RotationComponent>(entity).rotation;
		auto& uboComponent = cameraEntities.get<UboViewProjectionComponent>(entity);
		auto& cameraComponent = cameraEntities.get<CameraComponent>(entity);

		glm::vec3 direction;
		CalculateDirection(&direction, rotation);
		Normalize(&direction);
		
		uboComponent.view = lookAt(position, position + direction, cameraComponent.upAxis);
		auto projection = glm::perspective(glm::radians(cameraComponent.fov), 
			screenAspectRatio, cameraComponent.zNear, cameraComponent.zFar);
		projection[1][1] *= -1;
		
		uboComponent.proj = projection;
	}

	auto allProjections = ecs->registry->view<PositionComponent, RotationComponent, UboViewProjectionComponent>(entt::exclude<CameraComponent>);
	for (auto entity : allProjections)
	{
		auto& position = allProjections.get<PositionComponent>(entity).position;
		auto& rotation = allProjections.get<RotationComponent>(entity).rotation;
		auto& uboComponent = allProjections.get<UboViewProjectionComponent>(entity);

		glm::vec3 direction;
		CalculateDirection(&direction, rotation);
		Normalize(&direction);

		uboComponent.view = lookAt(position, position + direction, glm::vec3(0, 1, 0));
		const auto projection = glm::perspective(glm::radians(60.0f),
		                                         screenAspectRatio, 50.0f, 200.0f);
		
		uboComponent.proj = projection;
	}
}
