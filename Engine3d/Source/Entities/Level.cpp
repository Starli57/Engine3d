#include "Pch.h"
#include "Level.h"

#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/MeshContainer.h"

#include "Architecture/Ref.h"
#include "spdlog/spdlog.h"

Level::Level(entt::registry& ecs, Rollback* rollback)
{
	this->ecs = &ecs;
	this->rollback = rollback;
}

Level::~Level()
{
	rollback->Dispose();
}

void Level::LoadLevel()
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

	auto entity = ecs->create();

	auto triangleMesh1 = CreateRef<Mesh>(vertices, indices);
	auto triangle1 = CreateRef<Entity>(ecs);
	triangle1->AddComponent<Transform>();
	triangle1->AddComponent<MeshContainer>(triangleMesh1);

	//mesh2
	auto vertices2 = CreateRef<std::vector<Vertex>>();
	vertices2->reserve(3);
	vertices2->push_back(Vertex(glm::vec3(1.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices2->push_back(Vertex(glm::vec3(1.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices2->push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	auto indices2 = CreateRef<std::vector<uint32_t>>();
	indices2->push_back(0);
	indices2->push_back(1);
	indices2->push_back(2);

	auto triangleMesh2 = CreateRef<Mesh>(vertices2, indices2);
	auto triangle2 = CreateRef<Entity>(ecs);
	triangle2->AddComponent<Transform>();
	triangle2->AddComponent<MeshContainer>(triangleMesh2);

	//camera1
	auto cameraEntity = CreateRef<Entity>(ecs);
	auto cameraTransform = CreateRef<Transform>(glm::vec3(1, 1, 5), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	cameraEntity->AddComponent<Camera>(cameraTransform, 60, 1);//todo: set real screen aspect ration

	rollback->Add([this]() { UnloadLevel(); });
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");
	//todo: add unload logic
}
