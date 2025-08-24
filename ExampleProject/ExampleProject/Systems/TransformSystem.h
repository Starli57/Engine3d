#pragma once

#include <chrono>
#include "EngineCore/Core/ISystem.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

class TransformSystem : public ISystem
{
public:
	TransformSystem(const Ref<Ecs>& ecs);
	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};
