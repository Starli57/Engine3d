#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include "Renderer/Model/UboViewProjection.h"

#include "SharedLib/Ref.h"

class Camera
{
public:
	UboViewProjection GetUbo();

	Camera(Ref<Transform> transform, float pov, float screenAspectRatio);
	~Camera();

	void UpdateScreenAspectRatio(float screenAspectRatio);
	void UpdateUbo();


private:
	Ref<Transform> transform;

	UboViewProjection uboViewProjection;

	float pov = 60;
	float screenAspectRatio = 1;

	float zNear = 0.1f;
	float zFar = 1000;

};

