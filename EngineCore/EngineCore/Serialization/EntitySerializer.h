#pragma once

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <filesystem>

#include <EngineCore/Core/Ecs.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Core/AssetsDatabase.h"

#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/CameraFreeComponent.h"
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

class EntitySerializer
{
public:

	EntitySerializer(const Ref<ProjectSettings>& projectSettings, const Ref<AssetsDatabase>& assetsDatabase);
	~EntitySerializer();

	void SerializeWorld(const Ref<Ecs>& ecs, const std::string& filePath);
	bool InstantiateWorld(const Ref<Ecs>& ecs, const std::filesystem::path& filePath);

	void SerializePrefab(const Ref<Entity>& entity, const std::string& filePath);
	void SerializeEntity(const Ref<Entity>& entity, YAML::Emitter& emitter);

	bool InstantiatePrefab(const Ref<Ecs>& ecs, const std::filesystem::path& filePath);
	bool InstantiatePrefab(const Ref<Ecs>& ecs, YAML::Node& node);

	void InstantiateCombinedMesh(const Ref<Ecs>& ecs, const std::filesystem::path& filePath);

private:

	Ref<ProjectSettings> projectSettings;
	Ref<AssetsDatabase> assetsDatabase;

	template <typename T>
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity)
	{
		if (!entity->HasComponent<T>()) return;
		auto& component = entity->GetComponent<T>();

		SerializeComponent(out, entity, component);
	}

	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const NameComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const PositionComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const RotationComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const RotationVelocityComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const ScaleComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const CameraComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const CameraFreeComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const MeshComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const MaterialComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboModelComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboWorldComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const UboDiffuseLightComponent& component) const;
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, IdComponent& component) const;

	void InstantiateComponentName(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentPosition(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentRotation(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentRotationVelocity(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentScale(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentCamera(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentCameraFree(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentMesh(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentMaterial(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentUboModel(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentUboViewProjection(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentUboDiffuseLight(const Ref<Entity>& entity, YAML::Node node) const;
	void InstantiateComponentId(const Ref<Entity>& entity) const;
};
