#include "Pch.h"

#include "Level.h"
#include "Architecture/Ref.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/MeshContainer.h"

#include "spdlog/spdlog.h"

Level::Level(Ref<entt::registry> ecs, IGraphicsApi* graphicsApi, Rollback* rollback)
{
	this->ecs = ecs;
	this->graphicsApi = graphicsApi;
	this->rollback = new Rollback("Level", *rollback);
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

	auto triangleMesh1 = graphicsApi->CreateMesh(vertices, indices);
	auto triangle1 = CreateRef<Entity>(ecs);
	triangle1->AddComponent<Transform>(glm::vec3(-1, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	triangle1->AddComponent<MeshContainer>(triangleMesh1);

	auto triangleMesh2 = graphicsApi->CreateMesh(vertices, indices);
	auto triangle2 = CreateRef<Entity>(ecs);
	triangle2->AddComponent<Transform>(glm::vec3(1, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	triangle2->AddComponent<MeshContainer>(triangleMesh2);

	//camera1
	auto cameraEntity = CreateRef<Entity>(ecs);
	auto cameraTransform = CreateRef<Transform>(glm::vec3(0, 1, 5), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	cameraEntity->AddComponent<Camera>(cameraTransform, 60, 1);//todo: set real screen aspect ration

	rollback->Add([this]() { UnloadLevel(); });
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");
	ecs->clear();
}
