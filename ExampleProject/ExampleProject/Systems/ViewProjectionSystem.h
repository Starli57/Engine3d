#pragma once

#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Core/ISystem.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Utilities/MathUtility.h"

class ViewProjectionSystem : public ISystem
{
public:
	ViewProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window);
	virtual void Update(float deltaTime) override;
	
private:
	Ref<Ecs> ecs;
	GLFWwindow* window;
};

