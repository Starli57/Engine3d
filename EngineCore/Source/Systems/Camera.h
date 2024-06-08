#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ISystem.h"

#include "EngineShared/Components/PositionComponent.h"
#include "EngineShared/Components/UboViewProjectionComponent.h"
#include "EngineShared/Components/CameraComponent.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"

class Camera : public ISystem
{
public:
	Camera(Ref<Ecs> ecs, GLFWwindow* window);
	virtual ~Camera() override;

	virtual void Update(float deltaTime) override;


private:
	Ref<Ecs> ecs;
	GLFWwindow* window;
};

