
#include "EngineCore/Pch.h"
#include "EngineCore/Core/Ecs.h"

#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/UboModelComponent.h"

#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"

Ecs::Ecs()
{
	allEntities = std::vector<Ref<Entity>>();
	registry = CreateRef<entt::registry>();
}

Ecs::~Ecs()
{
	allEntities.clear();
}

Ref<Entity> Ecs::CreateEntity()
{
	auto entity = CreateRef<Entity>(registry);

	allEntities.push_back(entity);
	return entity;
}

void Ecs::DestroyEntiy(Ref<Entity> entity)
{
	allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), entity), allEntities.end());
	registry->destroy(entity->GetEntity());
}
