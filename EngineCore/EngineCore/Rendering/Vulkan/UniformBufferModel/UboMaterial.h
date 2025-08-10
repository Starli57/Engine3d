#pragma once

#include <glm/glm.hpp>

namespace AVulkan
{
    struct UboMaterial
    {
        glm::vec4 baseColorFactor;
        float metallicFactor;
        float roughnessFactor;
        float alphaCutoffFactor;
        
        UboMaterial(const Ref<PbrMaterial>& material) : 
            baseColorFactor(material->baseColor), metallicFactor(material->metallic),
            roughnessFactor(material->roughness), alphaCutoffFactor(material->alphaCutoff)
        {
            
        }
    };
}