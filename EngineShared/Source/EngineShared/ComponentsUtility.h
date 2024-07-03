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

void AddComponent(Ref<Entity> entity, const std::string& componentName)
{
	if (componentName == "PositionComponent") entity->AddComponent<PositionComponent>();
	if (componentName == "RotationComponent") entity->AddComponent<RotationComponent>();
	if (componentName == "ScaleComponent") entity->AddComponent<ScaleComponent>();
}