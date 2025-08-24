#pragma once

#include "EngineCore/Assets/MeshAsset.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"

struct CubeDefinition : public MeshAsset
{
    CubeDefinition()
    {
        auto whiteColor = glm::vec3(1.0f, 1.0f, 1.0f);
        auto vec3zero = glm::vec3(0, 0, 0);

        vertices = std::vector<Vertex>
        {
            Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), vec3zero, whiteColor, glm::vec3(1.0f, 0.0f, 0.0f)), // 0: Front-bottom-left
            Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), vec3zero, whiteColor, glm::vec3(0.0f, 1.0f, 0.0f)), // 1: Front-bottom-right
            Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), vec3zero, whiteColor, glm::vec3(0.0f, 0.0f, 1.0f)), // 2: Front-top-right
            Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), vec3zero, whiteColor, glm::vec3(1.0f, 1.0f, 0.0f)), // 3: Front-top-left
            Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), vec3zero, whiteColor, glm::vec3(1.0f, 0.0f, 1.0f)), // 4: Back-bottom-left
            Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), vec3zero, whiteColor, glm::vec3(1.0f, 1.0f, 1.0f)), // 5: Back-bottom-right
            Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), vec3zero, whiteColor, glm::vec3(0.5f, 0.5f, 0.5f)), // 6: Back-top-right
            Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), vec3zero, whiteColor, glm::vec3(0.0f, 0.5f, 0.5f))  // 7: Back-top-left
        };

        indices = std::vector<uint32_t>
        {
            // Front face
            0, 1, 2, 2, 3, 0,

            // Right face
            1, 5, 6, 6, 2, 1,

            // Back face
            5, 4, 7, 7, 6, 5,

            // Left face
            4, 0, 3, 3, 7, 4,

            // Top face
            3, 2, 6, 6, 7, 3,

            // Bottom face
            4, 5, 1, 1, 0, 4,
        };

        std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f));

        for (size_t i = 0; i < indices.size(); i += 3) 
        {
            uint32_t index0 = indices.at(i);
            uint32_t index1 = indices.at(i + 1);
            uint32_t index2 = indices.at(i + 2);

            glm::vec3 normal = ComputeNormal(vertices.at(index0).position, vertices.at(index1).position, vertices.at(index2).position);

            normals[index0] += normal;
            normals[index1] += normal;
            normals[index2] += normal;
        }

        for (size_t i = 0; i < vertices.size(); ++i) 
        {
            vertices.at(i).normal = glm::normalize(normals[i]);
        }
    }
};

