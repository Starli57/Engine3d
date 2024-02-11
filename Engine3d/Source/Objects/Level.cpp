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

	entities = new std::vector<Entity*>();
	components = new std::unordered_map<std::string, std::vector<EntityComponent>>();
}

Level::~Level()
{
	rollback->Dispose();

	//todo: dispose entities
	//todo: dispose components
}

void Level::LoadLevel()
{
	spdlog::info("Load level");

	Entity* camera = new Entity("Camera");
	Camera* cameraComponent = new Camera(60, 1);//todo: set real screen aspect

	Entity* triangle = new Entity("Triangle1");
	Transform* triangleTransform = new Transform();
	std::vector<Vertex>* vertices = new std::vector<Vertex>();
	vertices->reserve(3);
	vertices->push_back(Vertex(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	std::vector<uint32_t>* indices = new std::vector<uint32_t>();
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	Mesh* triangle1Mesh = new Mesh(vertices, indices);


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
	entities->push_back(entity);
}

void Level::RemoveEntity(Entity* entity)
{
	entities->erase(std::remove(entities->begin(), entities->end(), entity->GetUniqueId()), entities->end());
}

void Level::AddComponent(Entity* entity, IComponent* component)
{
	EntityComponent ec{ entity, component };
	entity->AddComponent(component);
	components->at(component->GetType()).push_back(ec);
}

void Level::RemoveComponent(Entity* entity, IComponent* component)
{
	throw std::runtime_error("Not implemented");
}

void Level::RemoveComponent(Entity* entity, std::string componentType)
{
	throw std::runtime_error("Not implemented");
}
