#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct ScaleComponent : public IComponent
{
	glm::vec3 scale;

	ScaleComponent() { scale = glm::vec3(1, 1, 1); }
	ScaleComponent(glm::vec3 scale) :scale(scale) {}
	//todo: use move constructor
};