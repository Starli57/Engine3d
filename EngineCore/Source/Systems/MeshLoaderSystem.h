#pragma once

#include "ISystem.h"

#include "Rendering/IGraphicsApi.h"

#include "EngineShared/Ref.h"
#include "EngineShared/Ecs.h"
#include "EngineShared/Components/MeshComponent.h"
#include "EngineShared/Components/MeshLoadRequest.h"
#include "EngineShared/Components/MeshUnloadRequest.h"

class MeshLoaderSystem : ISystem
{
public:
	MeshLoaderSystem(IGraphicsApi* graphicsApi, Ref<Ecs> ecs);
	~MeshLoaderSystem();

	void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	IGraphicsApi* graphicsApi;

	void Load(entt::entity& entity, MeshComponent& meshComponent);
	void Unload(entt::entity& entity, MeshComponent& meshComponent);
};

