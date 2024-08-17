#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Input.h"
#include "EngineCore/Systems/ISystem.h"
#include "EngineCore/Components/FreeCameraComponent.h"
#include "EngineCore/Components/PositionComponent.h"

class FreeCameraSystem : ISystem
{
public:
	FreeCameraSystem(Ref<Ecs> ecs, Ref<Input> input);
	~FreeCameraSystem();

	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	Ref<Input> input;
};