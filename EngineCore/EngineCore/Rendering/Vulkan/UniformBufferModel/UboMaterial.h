#pragma once

#include <glm/glm.hpp>

namespace VulkanApi
{
    struct UboMaterial
    {
        glm::vec4 baseColorFactor;
        float metallicFactor;
        float roughnessFactor;
        float alphaCutoffFactor;
        float normalScale;
        
        UboMaterial(const Ref<Engine::PbrMaterialAsset>& material) : 
            baseColorFactor(material->baseColor), metallicFactor(material->metallic),
            roughnessFactor(material->roughness), alphaCutoffFactor(material->alphaCutoff), normalScale(material->normalsScale)
        {
            
        }
    };
}