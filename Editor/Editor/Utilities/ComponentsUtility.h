
#pragma once
#include <string>
#include <vector>
#include <stdexcept>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/IComponent.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/NotDestroyableEntityComponent.h"
#include "EngineCore/Components/NotSerializableEntityComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/ShadowMapComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "ExampleProject/Components/CameraFreeComponent.h"

/// <summary>
/// This script is autogenerated
/// Don't make any changes in the file
/// </summary>

const int componentsLength = 16;
extern const char* allComponentsNames[componentsLength];


bool HasComponent(Ref<Entity> entity, const std::string& componentName);
void AddComponent(Ref<Entity> entity, const std::string& componentName);
void RemoveComponent(Ref<Entity> entity, const std::string& componentName);
