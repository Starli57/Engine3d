#pragma once

#include <entt.hpp>

#include "IComponent.h"
#include "Transform.h"

#include "SharedLib/Ref.h"
#include <Entities/Entity.h>

class Rotator : IComponent
{
public:
	Rotator(Ref<Entity> entity);

	void Update() override;

private:
	Ref<Entity> entity;
};

