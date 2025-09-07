#pragma once
#include "glm/vec3.hpp"

class ShadowMapComponent
{
public:
    float farObjectProjectionMultiplier;

    float zNear;
    float zFar;

    glm::vec3 upAxis;

    ShadowMapComponent() 
    {
        farObjectProjectionMultiplier = 5.0f;

        zNear = 1.0f;
        zFar = 1000;

        upAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    ShadowMapComponent(const float farObjectProjectionMultiplier, const float zNear, const float zFar, const glm::vec3 upAxis)
        : farObjectProjectionMultiplier(farObjectProjectionMultiplier), zNear(zNear), zFar(zFar), upAxis(upAxis)
    {
    }
	//todo: use move constructor
};
