#include "Pch.h"
#include "World.h"

#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/MeshContainer.h"

#include "Macroses/Ref.h"
#include "spdlog/spdlog.h"

World::World(entt::registry& ecs, Rollback* rollback)
{
	this->ecs = &ecs;
	this->rollback = rollback;
}

World::~World()
{
	rollback->Dispose();
}

void World::LoadLevel()
{
	spdlog::info("Load level");

	//mesh1
	auto vertices = CreateRef<std::vector<Vertex>>();
	vertices->reserve(3);
	vertices->push_back(Vertex(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	auto indices = CreateRef<std::vector<uint32_t>>();
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);

	auto triangleMesh1 = CreateRef<Mesh>(vertices, indices);
	auto triangle1 = CreateRef<Entity>(ecs);
	triangle1->AddComponent<Transform>(glm::vec3(-1, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	triangle1->AddComponent<MeshContainer>(triangleMesh1);

	auto triangleMesh2 = CreateRef<Mesh>(vertices, indices);
	auto triangle2 = CreateRef<Entity>(ecs);
	triangle2->AddComponent<Transform>(glm::vec3(1, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	triangle2->AddComponent<MeshContainer>(triangleMesh2);

	//camera1
	auto cameraEntity = CreateRef<Entity>(ecs);
	auto cameraTransform = CreateRef<Transform>(glm::vec3(0, 1, 4), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	cameraEntity->AddComponent<Camera>(cameraTransform, 60, 1);//todo: set real screen aspect ration

	rollback->Add([this]() { UnloadLevel(); });
}

void World::UnloadLevel()
{
	spdlog::info("Unload level");
	//todo: add unload logic
}
