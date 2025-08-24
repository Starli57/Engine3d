#pragma once

#include <glm/glm.hpp>
#include <numbers>
#include "EngineCore/Assets/MeshAsset.h"
using namespace EngineCore;

struct SphereDefinition : public MeshAsset
{
	SphereDefinition()
	{
        const uint32_t stackCount = 50;
        const uint32_t sectorCount = 50;
        const float radius = 1;
        const float pi = std::numbers::pi_v<float>;

        vertices.reserve(stackCount * sectorCount);
        for (uint32_t stack = 0; stack <= stackCount; ++stack) 
        {
            const float stackAngle = pi / 2 - stack * pi / stackCount;
            const float xy = radius * cosf(stackAngle);
            const float z = radius * sinf(stackAngle);

            for (uint32_t sector = 0; sector <= sectorCount; ++sector) 
            {
                const float sectorAngle = sector * 2 * pi / sectorCount;

                const float x = xy * cosf(sectorAngle);
                const float y = xy * sinf(sectorAngle);
                vertices.emplace_back(
                    glm::vec3(x, y, z), 
                    glm::vec3(x, y, z),
                    glm::vec3(1.0f), 
                    glm::vec2(static_cast<float>(sector) / sectorCount, static_cast<float>(stack) / stackCount));
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