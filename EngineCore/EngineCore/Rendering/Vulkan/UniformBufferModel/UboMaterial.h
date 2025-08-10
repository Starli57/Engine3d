#pragma once

#include <glm/glm.hpp>

namespace AVulkan
{
    struct UboMaterial
    {
        glm::vec4 baseColorFactor;

        UboMaterial(const Ref<PbrMaterial>& material) : 
            baseColorFactor(material->baseColor)
        {
            
        }
    };
}