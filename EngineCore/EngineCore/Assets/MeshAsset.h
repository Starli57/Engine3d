#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"
#include <string>

namespace Engine
{
    struct MeshAsset
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::string materialPath;
        std::string materialName;
        std::string meshName;

        glm::vec3 ComputeNormal(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2) const
        {
            const glm::vec3 edge1 = v1 - v0;
            const glm::vec3 edge2 = v2 - v0;
            return normalize(cross(edge1, edge2));
        }
        
        void CalculateBoundingBox(glm::vec3& minCoords, glm::vec3& maxCoords, glm::vec3& boundingBoxCenter) const
        {
            if (vertices.empty()) return;

            minCoords = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
            maxCoords = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

            for (const auto& vertex : vertices)
            {
                minCoords.x = std::min(minCoords.x, vertex.position.x);
                minCoords.y = std::min(minCoords.y, vertex.position.y);
                minCoords.z = std::min(minCoords.z, vertex.position.z);

                maxCoords.x = std::max(maxCoords.x, vertex.position.x);
                maxCoords.y = std::max(maxCoords.y, vertex.position.y);
                maxCoords.z = std::max(maxCoords.z, vertex.position.z);
            }

            boundingBoxCenter = (minCoords + maxCoords) * 0.5f;
        }
    };
}