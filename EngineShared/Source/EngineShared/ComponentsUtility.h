#pragma once

#include <vector>
#include <string>

#include "Ref.h"
#include "IComponent.h"

#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ScaleComponent.h"

const int componentsLength = 3;

const char* componentsNames[componentsLength] =
{
	"PositionComponent",
	"RotationComponent",
	"ScaleComponent"
};

bool HasComponent(Ref<Entity> entity, const std::string& componentName)
{
	if (componentName == "PositionComponent") return entity->HasComponent<PositionComponent>();
	if (componentName == "RotationComponent") return entity->HasComponent<RotationComponent>();
	if (componentName == "ScaleComponent") return entity->HasComponent<ScaleComponent>();
	throw std::runtime_error("Not supported component type: " + componentName);
}

void AddComponent(Ref<Entity> entity, const std::string& componentName)
{
	if (HasComponent(entity, componentName)) return;

	if (componentName == "PositionComponent") entity->AddComponent<PositionComponent>();
	if (componentName == "RotationComponent") entity->AddComponent<RotationComponent>();
	if (componentName == "ScaleComponent") entity->AddComponent<ScaleComponent>();
}
