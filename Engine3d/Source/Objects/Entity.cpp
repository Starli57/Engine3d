#include "Pch.h"

#include "Entity.h"
#include "Utilities/UniqueId.h"

#include "spdlog/spdlog.h"

Entity::Entity(std::string name)
{
	this->uniqueId = UniqueId::GetId();
	spdlog::info("Create Entity, id: " + uniqueId);

	this->name = name;
	this->components = new std::vector<IComponent*>();
}

Entity::~Entity()
{
	for (auto component : *components)
		delete component;
	delete components;
}

void Entity::AddComponent(IComponent* component)
{
	components->push_back(component);
}

void Entity::RemoveComponent(IComponent* component)
{
	RemoveComponent(GetComponentIndex(component->GetType()));
}

void Entity::RemoveComponent(std::string type)
{
	RemoveComponent(GetComponentIndex(type));
}

void Entity::RemoveComponent(int16_t index)
{
	if (index >= 0) components->erase(components->begin() + index);
}

bool Entity::HasComponent(std::string type)
{
	for (auto component : *components)
	{
		if (component->GetType() == type)
			return true;
	}
	return false;
}

int16_t Entity::GetComponentIndex(std::string type)
{
	for (int16_t i = 0; i < components->size(); i++)
	{
		if (components->at(i)->GetType() == type)
			return i;
	}
	return -1;
}

IComponent* Entity::GetComponent(std::string type)
{
	for (auto component : *components)
	{
		if (component->GetType() == type)
			return component;
	}
	return nullptr;
}

