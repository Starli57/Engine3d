#pragma once

#include <glm/glm.hpp>
#include "EngineCore/IComponent.h"

struct UboModelComponent : public IComponent
{
	glm::mat4 model;
};