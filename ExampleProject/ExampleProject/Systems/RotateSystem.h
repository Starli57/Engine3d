#pragma once

#include "EngineCore/EngineApi.h"
#include "EngineCore/Core/ISystem.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

class RotateSystem : ISystem
{
public:
	RotateSystem(const Ref<Ecs>& ecs);
	void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};

