#pragma once

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/ProjectSettings.h"

#include "EngineCore/Components/CameraComponent.h"
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

	EntitySerializer(Ref<ProjectSettigns> projectSettings);
	~EntitySerializer();

	void SerializePrefab(Ref<Entity> entity, const std::string& filePath);
	void InstantiatePrefab(const std::filesystem::path& path);

private:

	Ref<ProjectSettigns> projectSettings;

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
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboDiffuseLightComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MeshComponent& component);
	void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MaterialComponent& component);
};
