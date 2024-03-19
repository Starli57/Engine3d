#pragma once

#include <glm/glm.hpp>

struct RotationComponent
{
	glm::vec3 rotation;

	RotationComponent() {}
	RotationComponent(glm::vec3 rotation) :rotation(rotation) {}
};