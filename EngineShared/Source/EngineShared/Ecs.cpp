
#include "Ecs.h"

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
	//todo: add implementation
	std::runtime_error("Not implemented");
}
