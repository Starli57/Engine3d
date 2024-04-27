#pragma once

#include <entt.hpp>

#include "ISystem.h"

#include "SharedLib/Components/RotationComponent.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Entity.h"

class RotatorSystem : ISystem
{
public:
	RotatorSystem(Ref<entt::registry> ecs);

	void Update(float deltaTime) override;

private:
	Ref<entt::registry> ecs;
};

