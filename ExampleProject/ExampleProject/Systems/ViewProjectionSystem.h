#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Core/ISystem.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

class ViewProjectionSystem : public ISystem
{
public:
	ViewProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window);
	virtual void Update(float deltaTime) override;
	
private:
	Ref<Ecs> ecs;
	GLFWwindow* window;
};

