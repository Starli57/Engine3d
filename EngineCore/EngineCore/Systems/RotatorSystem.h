#pragma once

#include "ISystem.h"

#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

class RotatorSystem : ISystem
{
public:
	RotatorSystem(const Ref<Ecs>& ecs);

	void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};

