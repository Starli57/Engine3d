
#include "EngineCore/Pch.h"
#include "EngineCore/Utilities/ComponentsUtility.h"


/// <summary>
/// This script is autognerated
/// Don't make any changes in the file
/// </summary>

const char* allComponentsNames[componentsLength] =
{
      "CameraComponent",
      "MaterialComponent",
      "MeshComponent",
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
      if (componentName == "MaterialComponent") return entity->HasComponent<MaterialComponent>();
      if (componentName == "MeshComponent") return entity->HasComponent<MeshComponent>();
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
      if (componentName == "MaterialComponent") entity->AddComponent<MaterialComponent>();
      if (componentName == "MeshComponent") entity->AddComponent<MeshComponent>();
      if (componentName == "NameComponent") entity->AddComponent<NameComponent>();
      if (componentName == "PositionComponent") entity->AddComponent<PositionComponent>();
      if (componentName == "RotationComponent") entity->AddComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") entity->AddComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") entity->AddComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") entity->AddComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") entity->AddComponent<UboModelComponent>();
      if (componentName == "UboViewProjectionComponent") entity->AddComponent<UboViewProjectionComponent>();
}


void RemoveComponent(Ref<Entity> entity, const std::string& componentName)
{
      if (!HasComponent(entity, componentName)) return;

      if (componentName == "CameraComponent") entity->RemoveComponent<CameraComponent>();
      if (componentName == "MaterialComponent") entity->RemoveComponent<MaterialComponent>();
      if (componentName == "MeshComponent") entity->RemoveComponent<MeshComponent>();
      if (componentName == "NameComponent") entity->RemoveComponent<NameComponent>();
      if (componentName == "PositionComponent") entity->RemoveComponent<PositionComponent>();
      if (componentName == "RotationComponent") entity->RemoveComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") entity->RemoveComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") entity->RemoveComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") entity->RemoveComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") entity->RemoveComponent<UboModelComponent>();
      if (componentName == "UboViewProjectionComponent") entity->RemoveComponent<UboViewProjectionComponent>();
}

