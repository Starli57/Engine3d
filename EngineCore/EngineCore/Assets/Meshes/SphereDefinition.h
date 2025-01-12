#pragma once

#include <glm/glm.hpp>
#include <numbers>
#include "EngineCore/Assets/Meta/MeshMeta.h"

struct SphereDefinition : public MeshMeta
{
	SphereDefinition()
	{
        uint32_t stackCount = 50;
        uint32_t sectorCount = 50;
        float radius = 1;
        float pi = std::numbers::pi_v<float>;

        vertices.reserve(stackCount * sectorCount);
        for (uint32_t stack = 0; stack <= stackCount; ++stack) 
        {
            float stackAngle = pi / 2 - stack * pi / stackCount;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (uint32_t sector = 0; sector <= sectorCount; ++sector) 
            {
                float sectorAngle = sector * 2 * pi / sectorCount;

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                //todo: calculate tangent and bitangent
                vertices.emplace_back(
                    glm::vec3(x, y, z), 
                    glm::vec3(x, y, z),
                    glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, 0),
                    glm::vec3(1.0f), 
                    glm::vec2(float(sector) / sectorCount, float(stack) / stackCount));
            }
        }

        //todo: check capacity calculation
        indices.reserve(stackCount * sectorCount * 6 - stackCount * 2);
        for (uint32_t stack = 0; stack < stackCount; ++stack)
        {
            uint32_t k1 = stack * (sectorCount + 1);
            uint32_t k2 = k1 + sectorCount + 1;

            for (uint32_t sector = 0; sector < sectorCount; ++sector, ++k1, ++k2)
            {
                if (stack != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (stack != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
	}
};