#pragma once
#include <yaml-cpp/emitter.h>

#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Serialization/EntitySerializer.h"
#include "ExampleProject/Components/CameraFreeComponent.h"

namespace Client
{
	class EntitySerializer
	{
	public:
		EntitySerializer(const Ref<Engine::EntitySerializer>& enginesSerializer);
	
	private:
		void SerializeEntity(Ref<Entity>& entity, YAML::Emitter& emitter);
		bool InstantiatePrefab(Ref<Entity>& entity, YAML::Node& node) const;
    
		void SerializeComponent(YAML::Emitter& out, const CameraFreeComponent& component) const;
		void InstantiateComponentCameraFree(const Ref<Entity>& entity, YAML::Node node) const;

		template <typename T>
		void SerializeComponent(YAML::Emitter& out, Ref<Entity> entity)
		{
			if (!entity->HasComponent<T>()) return;
			auto& component = entity->GetComponent<T>();

			SerializeComponent(out, component);
		}

	};
}