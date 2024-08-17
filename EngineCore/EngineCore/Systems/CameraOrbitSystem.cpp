#include "EngineCore/Pch.h"
#include "CameraOrbitSystem.h"

CameraOrbitSystem::CameraOrbitSystem(Ref<Ecs> ecs, Ref<Input> input)
	: ecs(ecs), input(input)
{
}

CameraOrbitSystem::~CameraOrbitSystem()
{
}

void CameraOrbitSystem::Update(float deltaTime)
{
	auto entities = ecs->registry->view<PositionComponent, CameraOrbitComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& cam = entities.get<CameraOrbitComponent>(entity);

		position.x = glm::cos(glfwGetTime() * cam.angularSpeed) * cam.radius;
		position.z = glm::sin(glfwGetTime() * cam.angularSpeed) * cam.radius;
	}
}
