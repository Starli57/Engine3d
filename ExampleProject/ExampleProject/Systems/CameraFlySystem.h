#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Core/ISystem.h"

class CameraFlySystem : ISystem
{
public:
	CameraFlySystem(const Ref<Ecs>& ecs, const Ref<Engine::InputManager>& input);
	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	Ref<Engine::InputManager> input;
};