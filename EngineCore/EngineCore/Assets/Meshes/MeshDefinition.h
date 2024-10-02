#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"

struct MeshDefinition 
{
public:
    Ref<std::vector<Vertex>> vertices;
    Ref<std::vector<uint32_t>> indices;

    virtual ~MeshDefinition() = default;

    glm::vec3 ComputeNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
    {
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        return glm::normalize(glm::cross(edge1, edge2));
    }
};