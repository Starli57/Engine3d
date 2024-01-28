#include "Pch.h"
#include "Engine.h"

Engine::Engine()
{
#ifdef DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	spdlog::set_pattern("[%^%l%$] %v");

	std::vector<Vertex>* vertices = new std::vector<Vertex>();
	vertices->reserve(3);
	vertices->push_back(Vertex(glm::vec3( 0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices->push_back(Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	std::vector<uint32_t>* indices = new std::vector<uint32_t>();
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);

	engineRollback = new Rollback();
	renderer = new Renderer(*engineRollback);
	renderer->Init();

	auto mesh = new Mesh(vertices, indices);
	renderer->AddMesh(*mesh);
}

Engine::~Engine()
{
	engineRollback->Dispose();
	delete renderer;
}

void Engine::Run()
{
	renderer->Run();
}
