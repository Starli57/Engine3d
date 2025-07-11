#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct PositionComponent : public IComponent
{
	glm::vec3 position;

	PositionComponent() { position = glm::vec3(0, 0, 0); }
	PositionComponent(glm::vec3 position) :position(position) {}
	//todo: use move constructor
};