#include <catch_amalgamated.hpp>

#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Components/PositionComponent.h"

TEST_CASE("AddAndRemoveComponent", "[component]")
{
    auto ecs = CreateRef<Ecs>();
    auto entity = ecs->CreateEntity();

    REQUIRE(entity->HasComponent<PositionComponent>() == false);

    entity->AddComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);

    entity->RemoveComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == false);
}