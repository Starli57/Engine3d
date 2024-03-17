#pragma once

#include <glm/glm.hpp>

struct PositionComponent
{
	glm::vec3 position;

	PositionComponent() {}
	PositionComponent(glm::vec3 position) :position(position) {}
};