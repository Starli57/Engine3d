#pragma once

#include <glm/glm.hpp>
#include "EngineShared/IComponent.h"

struct UboViewProjectionComponent : public IComponent
{
    glm::mat4 view;
    glm::mat4 proj;
};