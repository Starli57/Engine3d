﻿#pragma once
#include "EngineCore/Core/ISystem.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Managers/InputManager.h"
#include "glm/vec3.hpp"
using namespace EngineCore;

class CameraLookAtSystem : ISystem
{
public:
    CameraLookAtSystem(const Ref<Ecs>& ecs, const Ref<InputManager>& input);
    virtual void Update(float deltaTime) override;
    
private:
    Ref<Ecs> ecs;
    Ref<InputManager> input;
};
