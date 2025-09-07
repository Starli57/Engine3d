#pragma once

namespace Engine
{
    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& viewProjection);
    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
}