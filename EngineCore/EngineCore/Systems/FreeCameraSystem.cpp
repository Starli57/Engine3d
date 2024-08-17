#pragma once

#include "EngineCore/Pch.h"
#include "FreeCameraSystem.h"

FreeCameraSystem::FreeCameraSystem(Ref<Ecs> ecs, Ref<Input> input)
	: ecs(ecs), input(input)
{
}

FreeCameraSystem::~FreeCameraSystem()
{
}

void FreeCameraSystem::Update(float deltaTime)
{
	auto entities = ecs->registry->view<PositionComponent, FreeCameraComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& freeCamComponent = entities.get<FreeCameraComponent>(entity);

		//todo: it's better to have separated component to set and apply position
		if (input->IsKeyPressed(GLFW_KEY_D))
			position.x += freeCamComponent.movementSpeed * deltaTime;
		if (input->IsKeyPressed(GLFW_KEY_A))
			position.x -= freeCamComponent.movementSpeed * deltaTime;
	}
}
