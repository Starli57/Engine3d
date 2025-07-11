#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct RotationVelocityComponent : public IComponent
{
	glm::vec3 velocity;

	RotationVelocityComponent() { velocity = glm::vec3(0, 0, 0); }
	RotationVelocityComponent(glm::vec3 velocity) :velocity(velocity) {}
	//todo: use move constructor
};