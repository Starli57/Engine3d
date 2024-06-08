#pragma once

#include <entt.hpp>

#include "ISystem.h"

#include "EngineShared/Components/RotationComponent.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"

class RotatorSystem : ISystem
{
public:
	RotatorSystem(Ref<entt::registry> ecs);

	void Update(float deltaTime) override;

private:
	Ref<entt::registry> ecs;
};

