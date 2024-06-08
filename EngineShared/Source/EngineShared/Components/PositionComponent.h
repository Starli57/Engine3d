#pragma once

#include <glm/glm.hpp>

struct PositionComponent
{
	glm::vec3 position;

	PositionComponent() { position = glm::vec3(0, 0, 0); }
	PositionComponent(glm::vec3 position) :position(position) {}
};