#pragma once

#include "EngineCore/Pch.h"
#include "CameraFreeSystem.h"

CameraFreeSystem::CameraFreeSystem(Ref<Ecs> ecs, Ref<Input> input)
	: ecs(ecs), input(input)
{
}

CameraFreeSystem::~CameraFreeSystem()
{
}

void CameraFreeSystem::Update(float deltaTime)
{
	auto entities = ecs->registry->view<PositionComponent, CameraFreeComponent>();
	for (auto entity : entities)
	{
		auto& position = entities.get<PositionComponent>(entity).position;
		auto& freeCamComponent = entities.get<CameraFreeComponent>(entity);

		//todo: it's better to have separated component to set and apply position
		if (input->IsKeyPressed(GLFW_KEY_D))
			position.x += freeCamComponent.movementSpeed * deltaTime;
		if (input->IsKeyPressed(GLFW_KEY_A))
			position.x -= freeCamComponent.movementSpeed * deltaTime;
	}
}
