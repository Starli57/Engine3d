#pragma once

#include <glm/glm.hpp>

namespace AVulkan
{
    struct UboMaterial
    {
        float transparency;

        UboMaterial(const Ref<Material>& material) : 
            transparency(material->transparency)
        {
            
        }
    };
}