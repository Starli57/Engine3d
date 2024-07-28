#pragma once

#include <glm/glm.hpp>
#include "EngineCore/IComponent.h"

struct UboViewProjectionComponent : public IComponent
{
    glm::mat4 view;
    glm::mat4 proj;
};