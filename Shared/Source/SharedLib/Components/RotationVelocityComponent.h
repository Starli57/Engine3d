#pragma once

#include <glm/glm.hpp>

struct RotationVelocityComponent
{
	glm::vec3 velocity;

	RotationVelocityComponent(glm::vec3 velocity) :velocity(velocity) {}
};