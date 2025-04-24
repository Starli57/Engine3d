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

EntitySerializer::EntitySerializer(const Ref<ProjectSettings>& projectSettings, const Ref<AssetsDatabase>& assetsDatabase) : 
	projectSettings(projectSettings), assetsDatabase(assetsDatabase)
{
}

EntitySerializer::~EntitySerializer()
{
}

void EntitySerializer::SerializeWorld(const Ref<Ecs>& ecs, const std::string& filePath)
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

bool EntitySerializer::InstantiateWorld(const Ref<Ecs>& ecs, const std::filesystem::path& filePath)
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

void EntitySerializer::SerializePrefab(const Ref<Entity>& entity, const std::string& filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;

	SerializeEntity(entity, out);

	out << YAML::EndMap;

	std::ofstream fout(filePath);
	fout << out.c_str();
}

void EntitySerializer::SerializeEntity(const Ref<Entity>& entity, YAML::Emitter& emitter)
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
	SerializeComponent<UboWorldComponent>(emitter, entity);
	SerializeComponent<UboDiffuseLightComponent>(emitter, entity);
}

bool EntitySerializer::InstantiatePrefab(const Ref<Ecs>& ecs, const std::filesystem::path& filePath)
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

bool EntitySerializer::InstantiatePrefab(const Ref<Ecs>& ecs, YAML::Node& node)
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

void EntitySerializer::InstantiateCombinedMesh(const Ref<Ecs>& ecs, const std::filesystem::path& filePath)
{
	std::vector<std::filesystem::path> meshPaths;
	std::vector<std::filesystem::path> materialPaths;

	std::ifstream file(filePath);
	if (file.is_open())
	{
		std::string meshPath;
		std::string materialPath;
		while (std::getline(file, meshPath) && std::getline(file, materialPath))
		{
			if (meshPath.empty() || materialPath.empty()) continue;
			meshPaths.push_back(std::filesystem::path(meshPath));
			materialPaths.push_back(std::filesystem::path(materialPath));
		}
		file.close();
	}

	for (int i = 0; i < meshPaths.size(); i++)
	{
		auto meshIndex = assetsDatabase->meshesIndexByPath.find(meshPaths.at(i));
		auto materialIndex = assetsDatabase->materialsIndexByPath.find(materialPaths.at(i));
		auto ent = ecs->CreateEntity();
		ent->AddComponent<IdComponent>();
		ent->AddComponent<PositionComponent>();
		ent->AddComponent<RotationComponent>();
		ent->AddComponent<ScaleComponent>();
		ent->AddComponent<MeshComponent>(meshIndex->second);
		ent->AddComponent<MaterialComponent>(materialIndex->second);
		ent->AddComponent<UboModelComponent>();
		ent->AddComponent<NameComponent>(meshPaths.at(i).filename().string());

		assetsDatabase->AddMeshLoadRequest(meshIndex->first);
		assetsDatabase->AddMaterialLoadRequest(materialIndex->first);
	}
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const NameComponent& component) const
{
	out << YAML::Key << "NameComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "name" << YAML::Value << component.name;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const PositionComponent& component) const
{
	out << YAML::Key << "PositionComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "position" << YAML::Value << component.position;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const RotationComponent& component) const
{
	out << YAML::Key << "RotationComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "rotation" << YAML::Value << component.rotation;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const RotationVelocityComponent& component) const
{
	out << YAML::Key << "RotationVelocityComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "velocity" << YAML::Value << component.velocity;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const ScaleComponent& component) const
{
	out << YAML::Key << "ScaleComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "scale" << YAML::Value << component.scale;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const CameraComponent& component) const
{
	out << YAML::Key << "CameraComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "fov" << YAML::Value << component.fov;
	out << YAML::Key << "zNear" << YAML::Value << component.zNear;
	out << YAML::Key << "zFar" << YAML::Value << component.zFar;
	out << YAML::Key << "upAxis" << YAML::Value << component.upAxis;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const CameraFreeComponent& component) const
{
	out << YAML::Key << "CameraFreeComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "movementSpeed" << YAML::Value << component.movementSpeed;
	out << YAML::Key << "rotationSpeed" << YAML::Value << component.rotationSpeed;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const UboDiffuseLightComponent& component) const
{
	out << YAML::Key << "UboDiffuseLightComponent";
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const MeshComponent& component) const
{
	out << YAML::Key << "MeshComponent";
	out << YAML::BeginMap;

	auto path = component.meshIndex.has_value() ? assetsDatabase->meshesPaths.at(component.meshIndex.value()).string() : "";
	out << YAML::Key << "meshPath" << YAML::Value << path;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, const MaterialComponent& component) const
{
	out << YAML::Key << "MaterialComponent";
	out << YAML::BeginMap;

	auto path = assetsDatabase->materialsPaths.at(component.materialIndex).string();
	out << YAML::Key << "materialPath" << YAML::Value << path;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboModelComponent& component) const
{
	out << YAML::Key << "UboModelComponent";
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboWorldComponent& component) const
{
	out << YAML::Key << "UboWorldComponent";
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, IdComponent& component) const
{
	//don't need to serialize, because ID is unique only inside 1 session
}

void EntitySerializer::InstantiateComponentName(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto nameComponent = node["NameComponent"])
	{
		entity->AddComponent<NameComponent>(nameComponent["name"].as<std::string>());
	}
}

void EntitySerializer::InstantiateComponentPosition(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto positionComponent = node["PositionComponent"])
	{
		entity->AddComponent<PositionComponent>(positionComponent["position"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentRotation(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto rotationComponent = node["RotationComponent"])
	{
		entity->AddComponent<RotationComponent>(rotationComponent["rotation"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentRotationVelocity(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto rotationVelocityComponent = node["RotationVelocityComponent"])
	{
		entity->AddComponent<RotationVelocityComponent>(rotationVelocityComponent["velocity"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentScale(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto scaleComponent = node["ScaleComponent"])
	{
		entity->AddComponent<ScaleComponent>(scaleComponent["scale"].as<glm::vec3>());
	}
}

void EntitySerializer::InstantiateComponentCamera(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto cameraComponent = node["CameraComponent"])
	{
		entity->AddComponent<CameraComponent>(
			cameraComponent["fov"].as<float>(),
			cameraComponent["zNear"].as<float>(),
			cameraComponent["zFar"].as<float>(),
			cameraComponent["upAxis"].as<glm::vec3>());
	}
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

void EntitySerializer::InstantiateComponentUboDiffuseLight(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto uboDiffuseLightComponent = node["UboDiffuseLightComponent"])
	{
		entity->AddComponent<UboDiffuseLightComponent>();
	}
}

void EntitySerializer::InstantiateComponentMesh(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto meshComponent = node["MeshComponent"])
	{
		auto path = meshComponent["meshPath"].as<std::string>();
		if (path.empty())
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
		assetsDatabase->AddMeshLoadRequest(index->first);
	}
}

void EntitySerializer::InstantiateComponentMaterial(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto materialComponent = node["MaterialComponent"])
	{
		auto path = materialComponent["materialPath"].as<std::string>();
		if (path.empty())
		{
			entity->AddComponent<MaterialComponent>();
			return;
		}


		const auto index = assetsDatabase->materialsIndexByPath.find(std::filesystem::path(path));
		if (index == assetsDatabase->materialsIndexByPath.end())
		{
			spdlog::critical("Failed to deserialize material component, because mterial by path {} not found", path);
			entity->AddComponent<MaterialComponent>();
			return;
		}

		entity->AddComponent<MaterialComponent>(index->second);
		assetsDatabase->AddMaterialLoadRequest(index->first);
	}
}

void EntitySerializer::InstantiateComponentUboModel(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto uboModelComponent = node["UboModelComponent"])
	{
		entity->AddComponent<UboModelComponent>();
	}
}

void EntitySerializer::InstantiateComponentUboViewProjection(const Ref<Entity>& entity, YAML::Node node) const
{
	if (auto uboViewProjection = node["UboWorldComponent"])
	{
		entity->AddComponent<UboWorldComponent>();
	}
}

void EntitySerializer::InstantiateComponentId(const Ref<Entity>& entity) const
{
	entity->AddComponent<IdComponent>(UniqueIdGenerator::Generate());
}
