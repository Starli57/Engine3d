#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct UboModelComponent : public IComponent
{
	glm::mat4 model;
};