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

EntitySerializer::EntitySerializer(Ref<ProjectSettigns> projectSettings) : projectSettings(projectSettings)
{
}

EntitySerializer::~EntitySerializer()
{
}

void EntitySerializer::SerializePrefab(Ref<Entity> entity, const std::string& filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;

	SerializeComponent<IdComponent>(out, entity);
	SerializeComponent<NameComponent>(out, entity);
	SerializeComponent<PositionComponent>(out, entity);
	SerializeComponent<RotationComponent>(out, entity);
	SerializeComponent<RotationVelocityComponent>(out, entity);
	SerializeComponent<ScaleComponent>(out, entity);
	SerializeComponent<CameraComponent>(out, entity);
	SerializeComponent<UboDiffuseLightComponent>(out, entity);
	SerializeComponent<MeshComponent>(out, entity);
	SerializeComponent<MaterialComponent>(out, entity);
	SerializeComponent<UboModelComponent>(out, entity);

	out << YAML::EndMap;

	std::ofstream fout(filePath);
	fout << out.c_str();
}

bool EntitySerializer::InstantiatePrefab(Ref<Ecs> ecs, const std::filesystem::path& path)
{
	YAML::Node data;

	try
	{
		data = YAML::LoadFile(path.string());
	}
	catch (YAML::ParserException e)
	{
		spdlog::critical("Failed to instantiate prefab path={0} error={1}", path.string(), e.what());
		return false;
	}

	auto entity = ecs->CreateEntity();
	entity->AddComponent<IdComponent>(UniqueIdGenerator::Generate());
	
	auto nameComponent = data["NameComponent"];
	if (nameComponent)
	{
		entity->AddComponent<NameComponent>(nameComponent["name"].as<std::string>());
	}

	auto positionComponent = data["PositionComponent"];
	if (positionComponent) 
	{
		entity->AddComponent<PositionComponent>(positionComponent["position"].as<glm::vec3>());
	}

	auto rotationComponent = data["RotationComponent"];
	if (rotationComponent)
	{
		entity->AddComponent<RotationComponent>(rotationComponent["rotation"].as<glm::vec3>());
	}

	auto rotationVelocityComponent = data["RotationVelocityComponent"];
	if (rotationVelocityComponent)
	{
		entity->AddComponent<RotationVelocityComponent>(rotationVelocityComponent["velocity"].as<glm::vec3>());
	}

	auto scaleComponent = data["ScaleComponent"];
	if (scaleComponent)
	{
		entity->AddComponent<ScaleComponent>(scaleComponent["scale"].as<glm::vec3>());
	}

	auto cameraComponent = data["CameraComponent"];
	if (cameraComponent)
	{
		entity->AddComponent<CameraComponent>(
			cameraComponent["fov"].as<float>(),
			cameraComponent["zNear"].as<float>(),
			cameraComponent["zFar"].as<float>(),
			cameraComponent["lookPoint"].as<glm::vec3>(),
			cameraComponent["upAxis"].as<glm::vec3>());
	}

	auto uboModelComponent = data["UboModelComponent"];
	if (uboModelComponent) 
	{
		entity->AddComponent<UboModelComponent>();
	}

	auto uboDiffuseLightComponent = data["UboDiffuseLightComponent"];
	if (uboDiffuseLightComponent)
	{
		entity->AddComponent<UboDiffuseLightComponent>(uboDiffuseLightComponent["position"].as<glm::vec3>());
	}

	auto meshComponent = data["MeshComponent"];
	if (meshComponent)
	{
		auto meshIndex = meshComponent["meshIndex"].as<int>();
		if (meshIndex == -1) entity->AddComponent<MeshComponent>();
		else entity->AddComponent<MeshComponent>(meshIndex);
	}

	auto materialComponent = data["MaterialComponent"];
	if (materialComponent)
	{
		entity->AddComponent<MaterialComponent>(materialComponent["materialIndex"].as<uint32_t>());
	}

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
	out << YAML::Key << "lookPoint" << YAML::Value << component.lookPoint;
	out << YAML::Key << "upAxis" << YAML::Value << component.upAxis;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboDiffuseLightComponent& component)
{
	out << YAML::Key << "UboDiffuseLightComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "position" << YAML::Value << component.position;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MeshComponent& component)
{
	out << YAML::Key << "MeshComponent";
	out << YAML::BeginMap;

	auto index = component.meshIndex.has_value() ? component.meshIndex.value() : -1;
	out << YAML::Key << "meshIndex" << YAML::Value << index;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, MaterialComponent& component)
{
	out << YAML::Key << "MaterialComponent";
	out << YAML::BeginMap;

	out << YAML::Key << "materialIndex" << YAML::Value << component.materialIndex;

	out << YAML::EndMap;
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, UboModelComponent& component)
{
	out << YAML::Key << "UboModelComponent";
}

void EntitySerializer::SerializeComponent(YAML::Emitter& out, Ref<Entity> entity, IdComponent& component)
{
	//don't need to serialize, because ID is unique only inside 1 session
}
