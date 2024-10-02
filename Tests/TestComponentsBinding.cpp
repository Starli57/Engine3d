#include <catch_amalgamated.hpp>

#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Components/PositionComponent.h"

TEST_CASE("AddComponent", "[component]")
{
    auto ecs = CreateRef<Ecs>();
    auto entity = ecs->CreateEntity();

    REQUIRE(entity->HasComponent<PositionComponent>() == false);

    entity->AddComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);
}

TEST_CASE("RemoveComponent", "[component]")
{
    auto ecs = CreateRef<Ecs>();
    auto entity = ecs->CreateEntity();

    REQUIRE(entity->HasComponent<PositionComponent>() == false);

    entity->AddComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);

    entity->RemoveComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == false);
}

TEST_CASE("AddIfNotExist", "[component]")
{
    auto ecs = CreateRef<Ecs>();
    auto entity = ecs->CreateEntity();

    REQUIRE(entity->HasComponent<PositionComponent>() == false);

    entity->AddComponentIfNotExist<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);

    entity->AddComponentIfNotExist<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);

    entity->RemoveComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == false);
}

TEST_CASE("AddOrReplace", "[component]")
{
    auto ecs = CreateRef<Ecs>();
    auto entity = ecs->CreateEntity();

    REQUIRE(entity->HasComponent<PositionComponent>() == false);

    entity->AddOrReplaceComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);

    entity->AddOrReplaceComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == true);

    entity->RemoveComponent<PositionComponent>();
    REQUIRE(entity->HasComponent<PositionComponent>() == false);
}