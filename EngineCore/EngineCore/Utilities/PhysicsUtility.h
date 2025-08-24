#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    glm::vec3 GetIntersectionPoint(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float distance);
    bool DoesIntersectCube(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& leftBottom, const glm::vec3& rightTop, float& distance);
    void CalculateRayFromScreen(double screenX, double screenY, float screenWidth, float screenHeight,
        const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3& rayOrigin, glm::vec3& rayDirection);
}