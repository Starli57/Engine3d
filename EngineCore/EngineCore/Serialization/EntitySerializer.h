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
#include "EngineCore/Components/UboViewProjectionComponent.h"

class EntitySerializer
{
public:

	EntitySerializer(Ref<ProjectSettings> projectSettings, Ref<AssetsDatabase> assetsDatabase);
	~EntitySerializer();

	void SerializeWorld(Ref<Ecs> ecs, const std::string& filePath);
	bool InstantiateWorld(Ref<Ecs> ecs, const std::filesystem::path& filePath);

	void SerializePrefab(Ref<Entity> entity, const std::string& filePath);
	void SerializeEntity(Ref<Entity> entity, YAML::Emitter& emitter);

	bool InstantiatePrefab(Ref<Ecs> ecs, const std::filesystem::path& filePath);
	bool InstantiatePrefab(Ref<Ecs> ecs, YAML::Node& node);

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

	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, NameComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, PositionComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, RotationComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, RotationVelocityComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, ScaleComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, CameraComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, CameraFreeComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MeshComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MaterialComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboModelComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboViewProjectionComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboDiffuseLightComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, IdComponent& component);

	void InstantiateComponentName(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentPosition(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentRotation(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentRotationVelocity(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentScale(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentCamera(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentCameraFree(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentMesh(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentMaterial(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentUboModel(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentUboViewProjection(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentUboDiffuseLight(Ref<Entity> entity, YAML::Node node);
	void InstantiateComponentId(Ref<Entity> entity);
};
