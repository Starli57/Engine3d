#pragma once

#include "Objects/Entity.h"
#include "ComponentsBase/IComponent.h"

struct EntityComponent
{
	Entity* entity;
	IComponent* component;

	EntityComponent(Entity* etity, IComponent* component)
	{
		this->entity = entity;
		this->component = component;
	}
};