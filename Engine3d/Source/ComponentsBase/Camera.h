#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>>

#include "IComponent.h"
#include "Transform.h"
#include "Rendering/Model/UboViewProjection.h"

class Camera : public IComponent
{
public:
	Camera(glm::vec3 position, glm::vec4 rotation, float pov, float screenAspectRatio);
	~Camera();

	void UpdateScreenAspectRatio(float screenAspectRatio);
	void UpdateUboViewProjection();

	UboViewProjection GetUboViewProjection();

	std::string GetType() override { return type; }
	void* GetComponent() override { return this; }

private:
	Transform* transform;

	UboViewProjection uboViewProjection;

	float pov;
	float screenAspectRatio;

	float zNear = 0.1f;
	float zFar = 1000;

	const std::string type = "Camera";
};

