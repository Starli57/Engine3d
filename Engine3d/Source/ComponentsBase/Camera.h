#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include "Rendering/Model/UboViewProjection.h"

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec4 rotation, float pov, float screenAspectRatio);
	~Camera();

	void UpdateScreenAspectRatio(float screenAspectRatio);
	void UpdateUboViewProjection();

	UboViewProjection GetUboViewProjection();

private:
	Transform* transform;

	UboViewProjection uboViewProjection;

	float pov;
	float screenAspectRatio;

	float zNear = 0.1f;
	float zFar = 1000;

};

