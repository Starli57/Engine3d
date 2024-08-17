#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct CameraOrbitComponent : IComponent
{
	float radius = 500;
	float angularSpeed = 1;
};