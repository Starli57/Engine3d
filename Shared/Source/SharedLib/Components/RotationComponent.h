#pragma once

#include <glm/glm.hpp>

struct RotationComponent
{
	glm::vec3 rotation;

	RotationComponent() { rotation = glm::vec3(0, 0, 0); }
	RotationComponent(glm::vec3 rotation) :rotation(rotation) {}
};