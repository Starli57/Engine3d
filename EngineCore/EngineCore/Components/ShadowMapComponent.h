#pragma once
#include "glm/vec3.hpp"

class ShadowMapComponent
{
public:
    float fov;

    float zNear;
    float zFar;

    glm::vec3 upAxis;

    ShadowMapComponent() 
    {
        fov = 60;

        zNear = 1.0f;
        zFar = 1000;

        upAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    ShadowMapComponent(const float fov, const float zNear, const float zFar, const glm::vec3 upAxis)
        : fov(fov), zNear(zNear), zFar(zFar), upAxis(upAxis)
    {
    }
	//todo: use move constructor
};
