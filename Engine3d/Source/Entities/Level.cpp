#include "Pch.h"

#include <tiny_obj_loader.h>

#include "Level.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/MeshContainer.h"
#include "Resources/TexturesList.h"
#include "Resources/MeshesList.h"

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

	auto vikingRoomTexture = graphicsApi->CreateTexture(textures[static_cast<size_t>(TextureId::formula1_Diffuse)]);
	assetDatabase->AddTexture(vikingRoomTexture);

	//todo: make dispose for textures better
	rollback->Add([this]() {assetDatabase->RemoveTexture(textures[static_cast<size_t>(TextureId::formula1_Diffuse)]); });

	auto vikingRoomMaterial = CreateRef<Material>(vikingRoomTexture);
	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	auto isLoaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshes[1].c_str());
	CAssert::Check(isLoaded, warn + err);

	auto vertices = CreateRef<std::vector<Vertex>>();
	auto indices = CreateRef<std::vector<uint32_t>>();
	for (const auto& shape : shapes) 
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex;
			
			vertex.position = 
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.uv = 
			{
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices->push_back(vertex);
			indices->push_back(indices->size());
		}
	}


	auto vikingRoomMesh = graphicsApi->CreateMesh(vertices, indices);
	auto vikingRoom = CreateRef<Entity>(ecs);
	vikingRoom->AddComponent<Transform>(glm::vec3(-0.5f, 0, -1), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	vikingRoom->AddComponent<MeshContainer>(vikingRoomMesh, vikingRoomMaterial);

	//camera1
	auto cameraEntity = CreateRef<Entity>(ecs);
	auto cameraTransform = CreateRef<Transform>(glm::vec3(0, 1, 500), glm::vec4(0, 0, 0, 0), glm::vec3(1, 1, 1));
	cameraEntity->AddComponent<Camera>(cameraTransform, 60, 1);//todo: set real screen aspect ration

	rollback->Add([this]() { UnloadLevel(); });
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");
	ecs->clear();
}
