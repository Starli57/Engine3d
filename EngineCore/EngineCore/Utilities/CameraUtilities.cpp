#include "EngineCore/Pch.h"
#include <glm/glm.hpp>
#include "CameraUtilities.h"

namespace Engine
{
    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
    {
        return GetFrustumCornersWorldSpace(proj * view);
    }
    
    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& viewProjection)
    {
        const auto inv = glm::inverse(viewProjection);
    
        std::vector<glm::vec4> frustumCorners;
        frustumCorners.reserve(8);
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }
    
        return frustumCorners;
    }
}