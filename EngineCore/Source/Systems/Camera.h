#pragma once

#include <GLFW/glfw3.h>
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
	//todo: replace pov and screen aspect to components
	Camera(Ref<entt::registry> ecs, GLFWwindow* window, float pov);
	virtual ~Camera() override;

	virtual void Update(float deltaTime) override;


private:
	Ref<entt::registry> ecs;
	GLFWwindow* window;

	float pov = 60;

	float zNear = 0.1f;
	float zFar = 1000;

};

