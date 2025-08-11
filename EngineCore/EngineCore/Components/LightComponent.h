#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct LightComponent : public IComponent
{
    glm::vec3 color;
    float intensity;

    LightComponent() : color(1.0f, 1.0f, 1.0f), intensity(1000.0f) {}
    LightComponent(glm::vec3 color, float intensity) : color(color), intensity(intensity) {}
};