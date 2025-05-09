
#include "EngineCore/Pch.h"
#include "EngineCore/Utilities/ComponentsUtility.h"


/// <summary>
/// This script is autognerated
/// Don't make any changes in the file
/// </summary>

const char* allComponentsNames[componentsLength] =
{
      "CameraComponent",
      "CameraFreeComponent",
      "CameraOrbitComponent",
      "IdComponent",
      "MaterialComponent",
      "MeshComponent",
      "NameComponent",
      "PositionComponent",
      "RotationComponent",
      "RotationVelocityComponent",
      "ScaleComponent",
      "UboDiffuseLightComponent",
      "UboModelComponent",
      "UboWorldComponent",
};


bool HasComponent(const Ref<Entity>& entity, const std::string& componentName)
{
      if (componentName == "CameraComponent") return entity->HasComponent<CameraComponent>();
      if (componentName == "CameraFreeComponent") return entity->HasComponent<CameraFreeComponent>();
      if (componentName == "CameraOrbitComponent") return entity->HasComponent<CameraOrbitComponent>();
      if (componentName == "IdComponent") return entity->HasComponent<IdComponent>();
      if (componentName == "MaterialComponent") return entity->HasComponent<MaterialComponent>();
      if (componentName == "MeshComponent") return entity->HasComponent<MeshComponent>();
      if (componentName == "NameComponent") return entity->HasComponent<NameComponent>();
      if (componentName == "PositionComponent") return entity->HasComponent<PositionComponent>();
      if (componentName == "RotationComponent") return entity->HasComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") return entity->HasComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") return entity->HasComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") return entity->HasComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") return entity->HasComponent<UboModelComponent>();
      if (componentName == "UboWorldComponent") return entity->HasComponent<UboWorldComponent>();
      throw std::runtime_error("Not supported component type: " + componentName);
}


void AddComponent(const Ref<Entity>& entity, const std::string& componentName)
{
      if (HasComponent(entity, componentName)) return;

      if (componentName == "CameraComponent") entity->AddComponent<CameraComponent>();
      if (componentName == "CameraFreeComponent") entity->AddComponent<CameraFreeComponent>();
      if (componentName == "CameraOrbitComponent") entity->AddComponent<CameraOrbitComponent>();
      if (componentName == "IdComponent") entity->AddComponent<IdComponent>();
      if (componentName == "MaterialComponent") entity->AddComponent<MaterialComponent>();
      if (componentName == "MeshComponent") entity->AddComponent<MeshComponent>();
      if (componentName == "NameComponent") entity->AddComponent<NameComponent>();
      if (componentName == "PositionComponent") entity->AddComponent<PositionComponent>();
      if (componentName == "RotationComponent") entity->AddComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") entity->AddComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") entity->AddComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") entity->AddComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") entity->AddComponent<UboModelComponent>();
      if (componentName == "UboWorldComponent") entity->AddComponent<UboWorldComponent>();
}


void RemoveComponent(const Ref<Entity>& entity, const std::string& componentName)
{
      if (!HasComponent(entity, componentName)) return;

      if (componentName == "CameraComponent") entity->RemoveComponent<CameraComponent>();
      if (componentName == "CameraFreeComponent") entity->RemoveComponent<CameraFreeComponent>();
      if (componentName == "CameraOrbitComponent") entity->RemoveComponent<CameraOrbitComponent>();
      if (componentName == "IdComponent") entity->RemoveComponent<IdComponent>();
      if (componentName == "MaterialComponent") entity->RemoveComponent<MaterialComponent>();
      if (componentName == "MeshComponent") entity->RemoveComponent<MeshComponent>();
      if (componentName == "NameComponent") entity->RemoveComponent<NameComponent>();
      if (componentName == "PositionComponent") entity->RemoveComponent<PositionComponent>();
      if (componentName == "RotationComponent") entity->RemoveComponent<RotationComponent>();
      if (componentName == "RotationVelocityComponent") entity->RemoveComponent<RotationVelocityComponent>();
      if (componentName == "ScaleComponent") entity->RemoveComponent<ScaleComponent>();
      if (componentName == "UboDiffuseLightComponent") entity->RemoveComponent<UboDiffuseLightComponent>();
      if (componentName == "UboModelComponent") entity->RemoveComponent<UboModelComponent>();
      if (componentName == "UboWorldComponent") entity->RemoveComponent<UboWorldComponent>();
}

