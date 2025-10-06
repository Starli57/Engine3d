#pragma once
#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ISystem.h"

class DirectionalShadowMapProjectionSystem: public ISystem
{
public:
    DirectionalShadowMapProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window);
    virtual void Update(float deltaTime) override;
	
private:
    Ref<Ecs> ecs;
    GLFWwindow* window;
};
