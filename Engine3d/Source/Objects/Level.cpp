#include "Pch.h"
#include "Level.h"

#include "spdlog/spdlog.h"

std::vector<EntityComponent> Level::GetComponents(std::string type)
{
	auto it = components->find(type);
	return it != components->end() ? it->second : std::vector<EntityComponent>();
}

Level::Level(Rollback* rollback)
{
	this->rollback = rollback;
	meshes = new std::vector<Mesh*>();
}

Level::~Level()
{
	rollback->Dispose();
}

void Level::LoadLevel()
{
	spdlog::info("Load level");

	std::vector<Vertex>* vertices = new std::vector<Vertex>();
	vertices->reserve(3);
	vertices->push_back(Vertex(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	std::vector<uint32_t>* indices = new std::vector<uint32_t>();
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);

	meshes->push_back(new Mesh(vertices, indices));

	std::vector<Vertex>* vertices2 = new std::vector<Vertex>();
	vertices2->reserve(3);
	vertices2->push_back(Vertex(glm::vec3(1.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices2->push_back(Vertex(glm::vec3(1.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices2->push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	std::vector<uint32_t>* indices2 = new std::vector<uint32_t>();
	indices2->push_back(0);
	indices2->push_back(1);
	indices2->push_back(2);

	meshes->push_back(new Mesh(vertices2, indices2));

	camera = new Camera(glm::vec3(1, 1, 5), glm::vec4(0, 0, 0, 0), 60, 1);

	rollback->Add([this]() { UnloadLevel(); });
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");

	delete camera;

	auto size = meshes->size();
	for (int i = 0; i < size; i++)
	{
		delete meshes->at(i);
	}
	delete meshes;
}

void Level::AddEntity(Entity* entity)
{
}

void Level::RemoveEntity(Entity* entity)
{
}

void Level::AddEntityComponent(Entity* entity, IComponent* component)
{
}

void Level::RemoveEntityComponent(Entity* entity, IComponent* component)
{
}

void Level::RemoveEntityComponent(Entity* entity, std::string componentType)
{

}

void Level::AddEntityComponent(std::vector<IComponent*>* componentsList, IComponent* component)
{
	componentsList->push_back(component);
}

void Level::RemoveEntityComponent(std::vector<IComponent*>* componentsList, IComponent* component)
{
	RemoveEntityComponent(GetComponentIndex(component->GetType()));
}

void Level::RemoveEntityComponent(std::vector<IComponent*>* componentsList, std::string componentType)
{

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