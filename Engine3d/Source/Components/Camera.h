#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include "Rendering/Model/Mvp.h"

#include "Macroses/Ref.h"

class Camera
{
public:
	UboViewProjection GetUboViewProjection();

	Camera(Ref<Transform> transform, float pov, float screenAspectRatio);
	~Camera();

	void UpdateScreenAspectRatio(float screenAspectRatio);
	void UpdateUboViewProjection();


private:
	Ref<Transform> transform;

	UboViewProjection uboViewProjection;

	float pov = 60;
	float screenAspectRatio = 1;

	float zNear = 0.1f;
	float zFar = 1000;

};

