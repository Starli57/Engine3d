#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"
#include <string>

struct MeshMeta
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::string materialPath;
    std::string materialName;
    std::string meshName;
    int materialIndex;//temp value used only by resources converter

    glm::vec3 ComputeNormal(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2) const
    {
        const glm::vec3 edge1 = v1 - v0;
        const glm::vec3 edge2 = v2 - v0;
        return normalize(cross(edge1, edge2));
    }
};