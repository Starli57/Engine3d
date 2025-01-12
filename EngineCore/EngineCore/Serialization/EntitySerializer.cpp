#include "EngineCore/Pch.h"
#include "EntitySerializer.h"
#include "EngineCore/Utilities/ComponentsUtility.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& v)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& v)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}
	};
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

EntitySerializer::EntitySerializer(Ref<ProjectSettings> projectSettings, Ref<AssetsDatabase> assetsDatabase) : 
	projectSettings(projectSettings), assetsDatabase(assetsDatabase)
{
}

EntitySerializer::~EntitySerializer()
{
}

void EntitySerializer::SerializeWorld(Ref<Ecs> ecs, const std::string& filePath)
{
	YAML::Emitter out;

	for (auto entity : ecs->allEntities)
	{
		out << YAML::BeginMap;
		SerializeEntity(entity, out);
		out << YAML::EndMap;
	}

	std::ofstream fout(filePath);
	fout << out.c_str();
}

bool EntitySerializer::InstantiateWorld(Ref<Ecs> ecs, const std::filesystem::path& filePath)
{
	std::vector<YAML::Node> data;

	try
	{
		data = YAML::LoadAllFromFile(filePath.string());
	}
	catch (YAML::ParserException e)
	{
		spdlog::critical("Failed to instantiate prefab path={0} error={1}", filePath.string(), e.what());
		return false;
	}

	for (auto node : data) 
		InstantiatePrefab(ecs, node);

	return true;
}

void EntitySerializer::SerializePrefab(Ref<Entity> entity, const std::string& filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;

	SerializeEntity(entity, out);

	out << YAML::EndMap;

	std::ofstream fout(filePath);
	fout << out.c_str();
}

void EntitySerializer::SerializeEntity(Ref<Entity> entity, YAML::Emitter& emitter)
{
	SerializeComponent<IdComponent>(emitter, entity);
	SerializeComponent<NameComponent>(emitter, entity);
	SerializeComponent<PositionComponent>(emitter, entity);
	SerializeComponent<RotationComponent>(emitter, entity);
	SerializeComponent<RotationVelocityComponent>(emitter, entity);
	SerializeComponent<ScaleComponent>(emitter, entity);
	SerializeComponent<CameraComponent>(emitter, entity);
	SerializeComponent<CameraFreeComponent>(emitter, entity);
	SerializeComponent<MeshComponent>(emitter, entity);
	SerializeComponent<MaterialComponent>(emitter, entity);
	SerializeComponent<UboModelComponent>(emitter, entity);
	SerializeComponent<UboViewProjectionComponent>(emitter, entity);
	SerializeComponent<UboDiffuseLightComponent>(emitter, entity);
}

bool EntitySerializer::InstantiatePrefab(Ref<Ecs> ecs, const std::filesystem::path& filePath)
{
	YAML::Node data;

	try
	{
		data = YAML::LoadFile(filePath.string());
	}
	catch (YAML::ParserException e)
	{
		spdlog::critical("Failed to instantiate prefab path={0} error={1}", filePath.string(), e.what());
		return false;
	}

	return InstantiatePrefab(ecs, data);
}

bool EntitySerializer::InstantiatePrefab(Ref<Ecs> ecs, YAML::Node& node)
{
	auto entity = ecs->CreateEntity();

	InstantiateComponentId(entity);
	InstantiateComponentName(entity, node);
	InstantiateComponentPosition(entity, node);
	InstantiateComponentRotation(entity, node);
	InstantiateComponentRotationVelocity(entity, node);
	InstantiateComponentScale(entity, node);
	InstantiateComponentCamera(entity, node);
	InstantiateComponentCameraFree(entity, node);
	InstantiateComponentUboModel(entity, node);
	InstantiateComponentUboViewProjection(entity, node);
	InstantiateComponentUboDiffuseLight(entity, node);
	InstantiateComponentMesh(entity, node);
	InstantiateComponentMaterial(entity, node);

	return true;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, NameComponent& component)
{
	out << YAML::Key << "NameComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "name" << YAML::Value << component.name;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, PositionComponent& component)
{
	out << YAML::Key << "PositionComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "position" << YAML::Value << component.position;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, RotationComponent& component)
{
	out << YAML::Key << "RotationComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "rotation" << YAML::Value << component.rotation;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, RotationVelocityComponent& component)
{
	out << YAML::Key << "RotationVelocityComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "velocity" << YAML::Value << component.velocity;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, ScaleComponent& component)
{
	out << YAML::Key << "ScaleComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "scale" << YAML::Value << component.scale;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, CameraComponent& component)
{
	out << YAML::Key << "CameraComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "fov" << YAML::Value << component.fov;
	out << YAML::Key << "zNear" << YAML::Value << component.zNear;
	out << YAML::Key << "zFar" << YAML::Value << component.zFar;
	out << YAML::Key << "upAxis" << YAML::Value << component.upAxis;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, CameraFreeComponent& component)
{
	out << YAML::Key << "CameraFreeComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "movementSpeed" << YAML::Value << component.movementSpeed;
	out << YAML::Key << "rotationSpeed" << YAML::Value << component.rotationSpeed;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboDiffuseLightComponent& component)
{
	out << YAML::Key << "UboDiffuseLightComponent";
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MeshComponent& component)
{
	out << YAML::Key << "MeshComponent";
	out << YAML::BeginMap;

	auto path = component.meshIndex.has_value() ? assetsDatabase->meshesPaths.at(component.meshIndex.value()).string() : "";
	out << YAML::Key << "meshPath" << YAML::Value << path;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MaterialComponent& component)
{
	out << YAML::Key << "MaterialComponent";
	out << YAML::BeginMap;

	auto path = assetsDatabase->materialsPaths.at(component.materialIndex).string();
	out << YAML::Key << "materialPath" << YAML::Value << path;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboModelComponent& component)
{
	out << YAML::Key << "UboModelComponent";
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboViewProjectionComponent& component)
{
	out << YAML::Key << "UboViewProjectionComponent";
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, IdComponent& component)
{
	//don't need to serialize, because ID is unique only inside 1 session
}

void EntitySerializer::InstantiateComponentName(Ref<Entity> entity, YAML::Node node)
{
	auto nameComponent = node["NameComponent"];
	if (nameComponent)
	{
		entity->AddComponent<NameComponent>(nameComponent["name"].as<std::string>());
	}
}

void EntitySerializer::InstantiateComponentPosition(Ref<Entity> entity, YAML::Node node)
{
	auto positionComponent = node["PositionComponent"];
	if (positionComponent)
	{
		entity->AddComponent<PositionComponent>(positionComponent["position"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentRotation(Ref<Entity> entity, YAML::Node node)
{
	auto rotationComponent = node["RotationComponent"];
	if (rotationComponent)
	{
		entity->AddComponent<RotationComponent>(rotationComponent["rotation"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentRotationVelocity(Ref<Entity> entity, YAML::Node node)
{
	auto rotationVelocityComponent = node["RotationVelocityComponent"];
	if (rotationVelocityComponent)
	{
		entity->AddComponent<RotationVelocityComponent>(rotationVelocityComponent["velocity"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentScale(Ref<Entity> entity, YAML::Node node)
{
	auto scaleComponent = node["ScaleComponent"];
	if (scaleComponent)
	{
		entity->AddComponent<ScaleComponent>(scaleComponent["scale"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentCamera(Ref<Entity> entity, YAML::Node node)
{
	auto cameraComponent = node["CameraComponent"];
	if (cameraComponent)
	{
		entity->AddComponent<CameraComponent>(
			cameraComponent["fov"].as<float>(),
			cameraComponent["zNear"].as<float>(),
			cameraComponent["zFar"].as<float>(),
			cameraComponent["upAxis"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentCameraFree(Ref<Entity> entity, YAML::Node node)
{
	auto cameraFreeComponent = node["CameraFreeComponent"];
	if (cameraFreeComponent)
	{
		entity->AddComponent<CameraFreeComponent>(
			cameraFreeComponent["movementSpeed"].as<float>(),
			cameraFreeComponent["rotationSpeed"].as<float>());
	}
}

void EntitySerializer::InstantiateComponentUboDiffuseLight(Ref<Entity> entity, YAML::Node node)
{
	auto uboDiffuseLightComponent = node["UboDiffuseLightComponent"];
	if (uboDiffuseLightComponent)
	{
		entity->AddComponent<UboDiffuseLightComponent>();
	}
}

void EntitySerializer::InstantiateComponentMesh(Ref<Entity> entity, YAML::Node node)
{
	auto meshComponent = node["MeshComponent"];
	if (meshComponent)
	{
		auto path = meshComponent["meshPath"].as<std::string>();
		if (path == "")
		{
			entity->AddComponent<MeshComponent>();
			return;
		}


		auto index = assetsDatabase->meshesIndexByPath.find(std::filesystem::path(path));
		if (index == assetsDatabase->meshesIndexByPath.end())
		{
			spdlog::critical("Failed to deserialize mesh component, because mesh by path {} not found", path);
			entity->AddComponent<MeshComponent>();
			return;
		}

		entity->AddComponent<MeshComponent>(index->second);
	}
}

void EntitySerializer::InstantiateComponentMaterial(Ref<Entity> entity, YAML::Node node)
{
	auto materialComponent = node["MaterialComponent"];
	if (materialComponent)
	{
		auto path = materialComponent["materialPath"].as<std::string>();
		if (path == "")
		{
			entity->AddComponent<MaterialComponent>();
			return;
		}


		auto index = assetsDatabase->materialsIndexByPath.find(std::filesystem::path(path));
		if (index == assetsDatabase->materialsIndexByPath.end())
		{
			spdlog::critical("Failed to deserialize material component, because mterial by path {} not found", path);
			entity->AddComponent<MaterialComponent>();
			return;
		}

		entity->AddComponent<MaterialComponent>(index->second);
	}
}

void EntitySerializer::InstantiateComponentUboModel(Ref<Entity> entity, YAML::Node node)
{
	auto uboModelComponent = node["UboModelComponent"];
	if (uboModelComponent)
	{
		entity->AddComponent<UboModelComponent>();
	}
}

void EntitySerializer::InstantiateComponentUboViewProjection(Ref<Entity> entity, YAML::Node node)
{
	auto uboViewProjection = node["UboViewProjectionComponent"];
	if (uboViewProjection)
	{
		entity->AddComponent<UboViewProjectionComponent>();
	}
}

void EntitySerializer::InstantiateComponentId(Ref<Entity> entity)
{
	entity->AddComponent<IdComponent>(UniqueIdGenerator::Generate());
}
