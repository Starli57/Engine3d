#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct RotationComponent : public IComponent
{
	glm::vec3 rotation;

	RotationComponent() { rotation = glm::vec3(0, 0, 0); }
	RotationComponent(const glm::vec3 rotation) :rotation(rotation) {}
};