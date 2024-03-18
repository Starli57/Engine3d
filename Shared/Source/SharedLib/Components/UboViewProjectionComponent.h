#pragma once

#include <glm/glm.hpp>

struct UboViewProjectionComponent
{
    glm::mat4 view;
    glm::mat4 proj;
};