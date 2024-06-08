#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ISystem.h"

#include "EngineShared/Components/PositionComponent.h"
#include "EngineShared/Components/UboViewProjectionComponent.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"

class Camera : public ISystem
{
public:
	//todo: replace pov and screen aspect to components
	Camera(Ref<Ecs> ecs, GLFWwindow* window, float pov);
	virtual ~Camera() override;

	virtual void Update(float deltaTime) override;


private:
	Ref<Ecs> ecs;
	GLFWwindow* window;

	float pov = 60;

	float zNear = 0.1f;
	float zFar = 1000;

};

