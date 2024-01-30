#include "Pch.h"
#include "Level.h"

#include "spdlog/spdlog.h"

std::vector<Mesh*>* Level::GetMeshes()
{
	return meshes;
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

	rollback->Add([this]() { UnloadLevel(); });
}

void Level::UnloadLevel()
{
	spdlog::info("Unload level");

	auto size = meshes->size();
	for (int i = 0; i < size; i++)
	{
		delete meshes->at(i);
	}
	delete meshes;
}
