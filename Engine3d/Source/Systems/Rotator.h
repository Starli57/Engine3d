#pragma once

#include <entt.hpp>

#include "ISystem.h"

#include "SharedLib/Components/RotationComponent.h"
#include "SharedLib/Ref.h"
#include "SharedLib/Entity.h"

class Rotator : ISystem
{
public:
	Rotator(Ref<entt::registry> ecs);

	void Update() override;

private:
	Ref<entt::registry> ecs;
};

