#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include "Rendering/Model/Mvp.h"

class Camera
{
public:
	UboViewProjection GetUboViewProjection();

	Camera(glm::vec3 position, glm::vec4 rotation, float pov, float screenAspectRatio);
	~Camera();

	void UpdateScreenAspectRatio(float screenAspectRatio);
	void UpdateUboViewProjection();


private:
	Transform* transform;

	UboViewProjection uboViewProjection;

	float pov;
	float screenAspectRatio;

	float zNear = 0.1f;
	float zFar = 1000;

};

