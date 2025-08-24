#pragma once

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <filesystem>

#include <EngineCore/Core/Ecs.h>
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/ProjectSettings.h"

#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/MaterialComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/ShadowMapComponent.h"
#include "EngineCore/Components/LightComponent.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"

namespace EngineCore
{
	class EntitySerializer
	{
	public:

		EntitySerializer(const Ref<ProjectSettings>& projectSettings, const Ref<ResourcesStorageVulkan>& resourcesStorage);

		void SerializeWorld(const Ref<Ecs>& ecs, const std::string& filePath);
		bool InstantiateWorld(const Ref<Ecs>& ecs, const std::filesystem::path& filePath) const;

		void SerializePrefab(Ref<Entity>& entity, const std::string& filePath);
		void SerializeEntity(Ref<Entity>& entity, YAML::Emitter& emitter);

		bool InstantiatePrefab(const Ref<Ecs>& ecs, const std::filesystem::path& filePath) const;
		bool InstantiatePrefab(const Ref<Ecs>& ecs, YAML::Node& node) const;

		void InstantiateCombinedMesh(const Ref<Ecs>& ecs, const std::filesystem::path& filePath) const;

		std::function<void(Ref<Entity>& entity, YAML::Emitter& emitter)> onSerializeClientEntities;
		std::function<void(Ref<Entity>& entity, YAML::Node& node)> onInstantiateClientEntities;
	
	private:

		Ref<ProjectSettings> projectSettings;
		Ref<ResourcesStorageVulkan> resourcesStorage;

		template <typename T>
		void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity)
		{
			if (!entity->HasComponent<T>()) return;
			auto& component = entity->GetComponent<T>();

			SerializeComponent(out, component);
		}

		void SerializeComponent(YAML::Emitter& out, const NameComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const PositionComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const RotationComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const RotationVelocityComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const ScaleComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const CameraComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const MeshComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const MaterialComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, UboModelComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, UboWorldComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const LightComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, IdComponent& component) const;
		void SerializeComponent(YAML::Emitter& out, const ShadowMapComponent& component) const;
	
		void InstantiateComponentName(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentPosition(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentRotation(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentRotationVelocity(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentScale(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentCamera(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentMesh(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentMaterial(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentUboModel(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentUboViewProjection(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentUboDiffuseLight(const Ref<Entity>& entity, YAML::Node node) const;
		void InstantiateComponentId(const Ref<Entity>& entity) const;
		void InstantiateComponentShadowMap(const Ref<Entity>& entity, YAML::Node node) const;
	};
}
