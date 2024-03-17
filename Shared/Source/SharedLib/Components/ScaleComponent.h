#pragma once

#include <glm/glm.hpp>

struct ScaleComponent
{
	glm::vec3 scale;

	ScaleComponent() {}
	ScaleComponent(glm::vec3 scale) :scale(scale) {}
};