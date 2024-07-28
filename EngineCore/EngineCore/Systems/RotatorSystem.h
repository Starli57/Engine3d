#pragma once

#include "ISystem.h"

#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Ecs.h"

class RotatorSystem : ISystem
{
public:
	RotatorSystem(Ref<Ecs> ecs);

	void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};

