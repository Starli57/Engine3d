#include <catch_amalgamated.hpp>

#include "EngineCore/Utilities/PhysicsUtility.h"

TEST_CASE("Test Intersection Hit")
{
    auto cubePosition = glm::vec3(5.0f, 0.0f, 0.0f);
    auto cubeSize = glm::vec3(1.0f, 1.0f, 1.0f);

    float distance = 0;
    bool intersect = EngineCore::DoesIntersectCube(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), cubePosition - cubeSize / 2.0f, cubePosition + cubeSize / 2.0f, distance);
    
    REQUIRE(intersect == true);
    REQUIRE(glm::abs(distance - 4.5f) < 0.01f);
}

TEST_CASE("Test Intersection Hit Point")
{
    auto rayOrigin = glm::vec3(0, 0, 0);
    auto rayDirection = glm::vec3(1, 0, 0);
    auto cubePosition = glm::vec3(5.0f, 0.0f, 0.0f);
    auto cubeSize = glm::vec3(1.0f, 1.0f, 1.0f);

    float distance = 0;
    bool intersect = EngineCore::DoesIntersectCube(rayOrigin, rayDirection, cubePosition - cubeSize / 2.0f, cubePosition + cubeSize / 2.0f, distance);
    auto intersectionPoint = EngineCore::GetIntersectionPoint(rayOrigin, rayDirection, distance);
    
    REQUIRE(intersect == true);
    REQUIRE(glm::distance(intersectionPoint, glm::vec3(cubePosition.x - cubeSize.x / 2, rayOrigin.y, cubePosition.z)) < 0.01f);
}

TEST_CASE("Test Intersection Miss")
{
    auto cubePosition = glm::vec3(5.0f, 0.0f, 0.0f);
    auto cubeSize = glm::vec3(1.0f, 1.0f, 1.0f);
    
    float distance = 0;
    bool intersect = EngineCore::DoesIntersectCube(glm::vec3(0, 0.5f, 0), glm::vec3(1, 1, 0), cubePosition - cubeSize / 2.0f, cubePosition + cubeSize / 2.0f, distance);
    
    REQUIRE(intersect == false);
}