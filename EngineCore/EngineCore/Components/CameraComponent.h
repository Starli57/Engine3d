#pragma once

#include <glm/glm.hpp>
#include "EngineCore/Core/IComponent.h"

struct CameraComponent : public IComponent
{
	float fov;

	float zNear;
	float zFar;

	glm::vec3 upAxis;

	CameraComponent() 
	{
		fov = 60;

		zNear = 0.1f;
		zFar = 5000;

		upAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	CameraComponent(const float fov, const float zNear, const float zFar, const glm::vec3 upAxis)
		: fov(fov), zNear(zNear), zFar(zFar), upAxis(upAxis)
	{
	}
};

