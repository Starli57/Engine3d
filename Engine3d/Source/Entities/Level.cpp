#include "Pch.h"

#include "Level.h"
#include "Architecture/Ref.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/MeshContainer.h"
#include "Resources/TexturesList.h"

#include "spdlog/spdlog.h"

Level::Level(Ref<entt::registry> ecs, Ref<AssetsDatabase> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback)
{
	this->ecs = ecs;
	this->assetDatabase = assetDatabase;
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
	vertices->push_back(Vertex(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0,0)));
	vertices->push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1,1)));
	vertices->push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0,1)));

	auto indices = CreateRef<std::vector<uint32_t>>();
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);

	auto defaultTexture = graphicsApi->CreateTexture(textures[static_cast<size_t>(TextureId::GroundDirtWeedsPatchy004_COL_2K)]);
	assetDatabase->AddTexture(defaultTexture);

	//todo: make dispose for textures better
	rollback->Add([this]() {assetDatabase->RemoveTexture(textures[static_cast<size_t>(TextureId::GroundDirtWeedsPatchy004_COL_2K)]); });

	auto defaultMateral = CreateRef<Material>(defaultTexture);

	auto triangleMesh1 = graphicsApi->CreateMesh(vertices, indices);
	auto triangle1 = CreateRef<Entity>(ecs);
	triangle1->AddComponent<Transform>(glm::vec3(-0.5f, 0, -1), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	triangle1->AddComponent<MeshContainer>(triangleMesh1, defaultMateral);

	auto triangleMesh2 = graphicsApi->CreateMesh(vertices, indices);
	auto triangle2 = CreateRef<Entity>(ecs);
	triangle2->AddComponent<Transform>(glm::vec3(0.5f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec3(2, 2, 2));
	triangle2->AddComponent<MeshContainer>(triangleMesh2, defaultMateral);

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
