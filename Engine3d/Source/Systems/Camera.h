#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ISystem.h"

#include "SharedLib/Entity.h"
#include "SharedLib/Components/PositionComponent.h"
#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "SharedLib/Ref.h"

class Camera : public ISystem
{
public:
	Camera(Ref<Entity> entity, float pov, float screenAspectRatio);
	virtual ~Camera() override;

	void UpdateScreenAspectRatio(float screenAspectRatio);
	virtual void Update() override;


private:
	Ref<Entity> entity;

	float pov = 60;
	float screenAspectRatio = 1;

	float zNear = 0.1f;
	float zFar = 1000;

};

