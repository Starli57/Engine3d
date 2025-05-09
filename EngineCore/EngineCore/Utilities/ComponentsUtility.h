
#pragma once
#include <string>
#include <stdexcept>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/IComponent.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/CameraFreeComponent.h"
#include "EngineCore/Components/CameraOrbitComponent.h"
#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"

/// <summary>
/// This script is autognerated
/// Don't make any changes in the file
/// </summary>

constexpr int componentsLength = 14;
extern const char* allComponentsNames[componentsLength];


bool HasComponent(const Ref<Entity>& entity, const std::string& componentName);
void AddComponent(const Ref<Entity>& entity, const std::string& componentName);
void RemoveComponent(const Ref<Entity>& entity, const std::string& componentName);
