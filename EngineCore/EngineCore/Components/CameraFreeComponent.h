#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct CameraFreeComponent : IComponent
{
	float movementSpeed = 250.0f;
	float rotationSpeed = 20.0f;

	CameraFreeComponent() {}

	CameraFreeComponent(float movementSpeed, float rotationSpeed)
		: movementSpeed(movementSpeed), rotationSpeed(rotationSpeed)
	{
	}
};