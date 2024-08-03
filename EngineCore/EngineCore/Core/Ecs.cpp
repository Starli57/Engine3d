
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

	SetupDependencies();
}

Ecs::~Ecs()
{
	allEntities.clear();
}

Ref<Entity> Ecs::CreateEntity()
{
	auto entity = CreateRef<Entity>(registry);
//	entity->AddComponent<PositionComponent>();
//	entity->AddComponent<RotationComponent>();
//	entity->AddComponent<ScaleComponent>();
//	entity->AddComponent<UboModelComponent>();

	allEntities.push_back(entity);
	return entity;
}

void Ecs::DestroyEntiy(Ref<Entity> entity)
{
	allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), entity), allEntities.end());
	registry->destroy(entity->GetEntity());
}

void Ecs::SetupDependencies()
{
//	registry->on_construct<MaterialComponent>().connect<&entt::registry::get_or_emplace<MeshComponent>>();
//	registry->on_construct<MeshComponent>().connect<&entt::registry::get_or_emplace<MaterialComponent>>();

//	registry->on_construct<RotationVelocityComponent>().connect < &entt::registry::get_or_emplace<RotationComponent>>();
}
