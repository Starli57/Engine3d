#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct UboWorldComponent : public IComponent
{
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
};