#pragma once

#include <glm/glm.hpp>
#include "EngineShared/IComponent.h"

struct UboModelComponent : public IComponent
{
	glm::mat4 model;
};