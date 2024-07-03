#pragma once

#include <glm/glm.hpp>
#include "EngineShared/IComponent.h"

struct RotationVelocityComponent : public IComponent
{
	glm::vec3 velocity;

	RotationVelocityComponent(glm::vec3 velocity) :velocity(velocity) {}
};