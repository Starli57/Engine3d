#pragma once

#include <chrono>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Core/ISystem.h"
using namespace EngineCore;

class CameraFlySystem : ISystem
{
public:
	CameraFlySystem(const Ref<Ecs>& ecs, const Ref<InputManager>& input);
	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	Ref<InputManager> input;
};