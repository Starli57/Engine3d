#include "EngineCore/Pch.h"
#include "PhysicsUtility.h"

namespace Engine
{
    glm::vec3 GetIntersectionPoint(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float distance)
    {
        return rayOrigin + rayDirection * distance;
    }

    bool DoesIntersectCube(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& leftBottom, const glm::vec3& rightTop, float& distance)
    {
        //todo: cache this block to avoid same calculations for multiple objects
        glm::vec3 direction;
        direction.x = 1.0f / rayDirection.x;
        direction.y = 1.0f / rayDirection.y;
        direction.z = 1.0f / rayDirection.z;

        auto t1 = (leftBottom.x - rayOrigin.x) * direction.x;
        auto t2 = (rightTop.x - rayOrigin.x) * direction.x;
        auto t3 = (leftBottom.y - rayOrigin.y) * direction.y;
        auto t4 = (rightTop.y - rayOrigin.y) * direction.y;
        auto t5 = (leftBottom.z - rayOrigin.z) * direction.z;
        auto t6 = (rightTop.z - rayOrigin.z) * direction.z;

        auto tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
        auto tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

        if (tmax < 0) return false;
        if (tmin > tmax) return false;

        distance = tmin;
        return true;
    };

    void CalculateRayFromScreen(double screenX, double screenY, float screenWidth, float screenHeight,
        const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3& rayOrigin, glm::vec3& rayDirection)
    {
        // 1. Convert screen coordinates to Normalized Device Coordinates (NDC)
        // NDC ranges from (-1, -1) to (1, 1)
        // screenX: 0 to screenWidth, screenY: 0 to screenHeight
        // Assuming screen origin is top-left. If bottom-left, adjust screenY calculation.
        float ndcX = 2.0f * screenX / screenWidth - 1.0f;
        float ndcY = 2.0f * screenY / screenHeight - 1.0f;

        // We need two points in NDC space to define the ray: one on the near plane and one on the far plane.
        // The Z values in NDC range from 0 to 1 (Vulkan/DirectX) or -1 to 1 (OpenGL).
        // Let's use 0 for the near plane and 1 for the far plane (common in Vulkan/DirectX, adjust if needed for OpenGL).
        glm::vec4 ndcNear = glm::vec4(ndcX, ndcY, 0.0f, 1.0f); // Z = 0 for near plane
        glm::vec4 ndcFar  = glm::vec4(ndcX, ndcY, 1.0f, 1.0f); // Z = 1 for far plane

        // 2. Convert NDC to World Space
        // We can combine the inverse projection and inverse view matrices.
        // Inverse(Projection * View) = Inverse(View) * Inverse(Projection)
        glm::mat4 inverseViewProjectionMatrix = glm::inverse(projectionMatrix * viewMatrix);

        // Transform the NDC points to World Space
        glm::vec4 worldNearHomogeneous = inverseViewProjectionMatrix * ndcNear;
        glm::vec4 worldFarHomogeneous  = inverseViewProjectionMatrix * ndcFar;

        // Perform the perspective divide to get 3D points in World Space
        // The W component contains the original W from clip space (related to view-space Z)
        glm::vec3 worldNear = glm::vec3(worldNearHomogeneous) / worldNearHomogeneous.w;
        glm::vec3 worldFar  = glm::vec3(worldFarHomogeneous) / worldFarHomogeneous.w;

        // 3. Calculate the ray direction
        // The ray starts at worldNear and goes towards worldFar
        rayOrigin = worldNear;
        rayDirection = glm::normalize(worldFar - worldNear);
    }
}