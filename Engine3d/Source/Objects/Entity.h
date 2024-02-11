#pragma once

#include <vector>

#include "Level.h"
#include "ComponentsBase/IComponent.h"

class Entity
{
public:
	Entity(Level* level, std::string name);
	~Entity();

	void AddComponent(IComponent* component);
	void RemoveComponent(IComponent* component);
	void RemoveComponent(std::string type);
	void RemoveComponent(int16_t index);

	bool HasComponent(std::string type);
	int16_t GetComponentIndex(std::string type);

	IComponent* GetComponent(std::string type);

	friend class Level;

private:
	Level* level;

	std::string name;
	std::vector<IComponent*>* components;
};

