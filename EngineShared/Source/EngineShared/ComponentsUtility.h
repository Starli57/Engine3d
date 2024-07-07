
#pragma once
#include <string>
#include <vector>
#include "Ref.h"
#include "IComponent.h"

#include "Components/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "Components/MeshLoadRequest.h"
#include "Components/MeshUnloadRequest.h"
#include "Components/NameComponent.h"
#include "Components/PositionComponent.h"
#include "Components/RotationComponent.h"
#include "Components/RotationVelocityComponent.h"
#include "Components/ScaleComponent.h"
#include "Components/UboDiffuseLightComponent.h"
#include "Components/UboModelComponent.h"
#include "Components/UboViewProjectionComponent.h"

/// <summary>
/// This script is autognerated
/// Don't make any changes in the file
/// </summary>

const int componentsLength = 12;

const char* componentsNames[componentsLength] =
{
      "CameraComponent",
      "MeshComponent",
      "MeshLoadRequest",
      "MeshUnloadRequest",
      "NameComponent",
      "PositionComponent",
      "RotationComponent",
      "RotationVelocityComponent",
      "ScaleComponent",
      "UboDiffuseLightComponent",
      "UboModelComponent",
      "UboViewProjectionComponent",
};


bool HasComponent(Ref<Entity> entity, const std::string& componentName)
{
      if (componentName == "CameraComponent") return entity->HasComponent<CameraComponent>();
      if (componentName == "MeshComponent") return entity->HasComponent<MeshComponent>();
      if (componentName == "MeshLoadRequest") return entity->HasComponent<MeshLoadRequest>();
      if (componentName == "MeshUnloadRequest") return entity->HasComponent<MeshUnloadRequest>();
      if (componentName == "NameComponent") return entity->HasComponent<NameComponent>();
      if (componentName == "PositionComponent") return entity->HasComponent<PositionComponent>();
      if (componentName == "RotationComponent") return entity->HasComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") return entity->HasComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") return entity->HasComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") return entity->HasComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") return entity->HasComponent<UboModelComponent>();
      if (componentName == "UboViewProjectionComponent") return entity->HasComponent<UboViewProjectionComponent>();
      throw std::runtime_error("Not supported component type: " + componentName);
}


void AddComponent(Ref<Entity> entity, const std::string& componentName)
{
       if (HasComponent(entity, componentName)) return;

      if (componentName == "CameraComponent") entity->AddComponent<CameraComponent>();
      if (componentName == "MeshComponent") entity->AddComponent<MeshComponent>();
      if (componentName == "MeshLoadRequest") entity->AddComponent<MeshLoadRequest>();
      if (componentName == "MeshUnloadRequest") entity->AddComponent<MeshUnloadRequest>();
      if (componentName == "NameComponent") entity->AddComponent<NameComponent>();
      if (componentName == "PositionComponent") entity->AddComponent<PositionComponent>();
      if (componentName == "RotationComponent") entity->AddComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") entity->AddComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") entity->AddComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") entity->AddComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") entity->AddComponent<UboModelComponent>();
      if (componentName == "UboViewProjectionComponent") entity->AddComponent<UboViewProjectionComponent>();
}

