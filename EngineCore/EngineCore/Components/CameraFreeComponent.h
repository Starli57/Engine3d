#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct CameraFreeComponent : IComponent
{
	float movementSpeed = 100.0f;
	float rotationSpeed = 100.0f;
};