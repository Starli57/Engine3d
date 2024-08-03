#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/UboViewProjectionComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Systems/ISystem.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

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

