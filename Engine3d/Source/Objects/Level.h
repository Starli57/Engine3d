#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Model/Mesh.h"
#include "ComponentsBase/Camera.h"
#include "Objects/Entity.h"
#include "Model/EntityComponent.h"

class Level
{
public:
	std::vector<EntityComponent> GetComponents(std::string type);

	Level(Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void AddEntityComponent(Entity* entity, IComponent* component);
	void RemoveEntityComponent(Entity* entity, IComponent* component);
	void RemoveEntityComponent(Entity* entity, std::string componentType);

private:
	Rollback* rollback;

	std::vector<Entity*>* entities;
	std::unordered_map<std::string, std::vector<EntityComponent>>* components;
};

