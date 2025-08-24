#include "EntitySerializer.h"

#include <yaml-cpp/node/node.h>

namespace Client
{
    EntitySerializer::EntitySerializer(const Ref<Engine::EntitySerializer>& enginesSerializer)
    {
        enginesSerializer->onSerializeClientEntities = [this](Ref<Entity>& entity, YAML::Emitter& emitter) { SerializeEntity(entity, emitter); };
        enginesSerializer->onInstantiateClientEntities = [this](Ref<Entity>& entity, YAML::Node& node) { InstantiatePrefab(entity, node); };
    }

    void EntitySerializer::SerializeEntity(Ref<Entity>& entity, YAML::Emitter& emitter)
    {
        SerializeComponent<CameraFreeComponent>(emitter, entity);
    }

    bool EntitySerializer::InstantiatePrefab(Ref<Entity>& entity, YAML::Node& node) const
    {
        InstantiateComponentCameraFree(entity, node);
        return true;
    }

    void EntitySerializer::SerializeComponent(YAML::Emitter& out, const CameraFreeComponent& component) const
    {
        out << YAML::Key << "CameraFreeComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "movementSpeed" << YAML::Value << component.movementSpeed;
        out << YAML::Key << "rotationSpeed" << YAML::Value << component.rotationSpeed;

        out << YAML::EndMap;
    }

    void EntitySerializer::InstantiateComponentCameraFree(const Ref<Entity>& entity, YAML::Node node) const
    {
        if (auto cameraFreeComponent = node["CameraFreeComponent"])
        {
            entity->AddComponent<CameraFreeComponent>(
                cameraFreeComponent["movementSpeed"].as<float>(),
                cameraFreeComponent["rotationSpeed"].as<float>());
        }
    }
}