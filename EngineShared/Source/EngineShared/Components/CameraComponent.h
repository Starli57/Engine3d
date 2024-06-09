#pragma once

#include <glm/glm.hpp>

struct CameraComponent
{
	float fov;

	float zNear;
	float zFar;

	glm::vec3 lookPoint;
	glm::vec3 upAxis;

	CameraComponent() 
	{
		fov = 60;

		zNear = 0.1f;
		zFar = 1000;

		lookPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		upAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	CameraComponent(float fov, float zNear, float zFar, glm::vec3 centerPoint, glm::vec3 upAxis) 
		: fov(fov), zNear(zNear), zFar(zFar), lookPoint(lookPoint), upAxis(upAxis)
	{
	}
};