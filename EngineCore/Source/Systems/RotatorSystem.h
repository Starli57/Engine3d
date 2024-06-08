#pragma once

#include "ISystem.h"

#include "EngineShared/Components/RotationComponent.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"

class RotatorSystem : ISystem
{
public:
	RotatorSystem(Ref<Ecs> ecs);

	void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};

