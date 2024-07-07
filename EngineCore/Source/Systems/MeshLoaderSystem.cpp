#include "Pch.h"
#include "MeshLoaderSystem.h"

MeshLoaderSystem::MeshLoaderSystem(IGraphicsApi* graphicsApi, Ref<Ecs> ecs) : graphicsApi(graphicsApi), ecs(ecs)
{
}

MeshLoaderSystem::~MeshLoaderSystem()
{
}

void MeshLoaderSystem::Update(float deltaTime)
{
	auto unloadEntities = ecs->registry->view<MeshComponent, MeshUnloadRequest>();
	auto loadEntities = ecs->registry->view<MeshComponent, MeshLoadRequest>();

	for (auto entity : unloadEntities)
	{
		auto& meshComponent = unloadEntities.get<MeshComponent>(entity);
		Unload(meshComponent);
	}

	for (auto entity : loadEntities)
	{
		auto& meshComponent = unloadEntities.get<MeshComponent>(entity);
		Load(entity, meshComponent);
	}
}

void MeshLoaderSystem::Load(entt::entity& entity, MeshComponent& meshComponent)
{
	if (meshComponent.meshPath.has_value())
	{
		auto mesh = graphicsApi->CreateMesh(meshComponent.meshPath.value());
		meshComponent.SetMesh(mesh);

		ecs->registry->remove<MeshLoadRequest>(entity);
	}
}

void MeshLoaderSystem::Unload(MeshComponent& meshComponent)
{
	throw std::runtime_error("Mesh unload is not implemented");
	//todo: need to delete UnloadRequest after that
}
