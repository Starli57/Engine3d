#pragma once
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"
#include "EngineShared/Components/NameComponent.h"
#include "EngineShared/Components/PositionComponent.h"
#include "EngineShared/Components/RotationComponent.h"
#include "EngineShared/Components/ScaleComponent.h"
#include "EngineShared/Components/RotationVelocityComponent.h"
#include "EngineShared/Components/CameraComponent.h"
#include "EngineShared/Components/ScaleComponent.h"
#include "EngineShared/Components/DiffuseLightComponent.h"

class IComponentInspector
{
public:
	virtual void Update(Ref<Entity> entity) = 0;
};

class NameInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<NameComponent>() == false) return;
		auto component = entity->GetComponent<NameComponent>();

		ImGui::SeparatorText("Name");
		ImGui::Text(component.name.c_str());
	}
};

class PositionInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<PositionComponent>() == false) return;
		auto component = &entity->GetComponent<PositionComponent>();

		ImGui::SeparatorText("Position");
		ImGui::DragFloat3("Position", glm::value_ptr(component->position), 0.1f);
	}
};

class RotationInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<RotationComponent>() == false) return;
		auto component = &entity->GetComponent<RotationComponent>();

		ImGui::SeparatorText("Rotation");
		ImGui::DragFloat3("Rotation", glm::value_ptr(component->rotation), 0.1f);
	}
};

class RotationVelocityInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<RotationVelocityComponent>() == false) return;
		auto component = &entity->GetComponent<RotationVelocityComponent>();

		ImGui::SeparatorText("Rotation Velocity");
		ImGui::DragFloat3("Velocity", glm::value_ptr(component->velocity), 0.1f);
	}
};

class ScaleInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<ScaleComponent>() == false) return;
		auto component = &entity->GetComponent<ScaleComponent>();

		ImGui::SeparatorText("Scale");
		ImGui::DragFloat3("Scale", glm::value_ptr(component->scale), 0.01f, 0, FLT_MAX);
	}
};

class CameraInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<CameraComponent>() == false) return;
		auto component = &entity->GetComponent<CameraComponent>();

		ImGui::SeparatorText("Camera");
		ImGui::DragFloat("FOV", &component->fov, 0.1f, 30, 120);
		ImGui::DragFloat("ZNear", &component->zNear, 0.1f);
		ImGui::DragFloat("ZFar", &component->zFar, 0.1f);
		ImGui::DragFloat3("Look at", glm::value_ptr(component->lookPoint));
		ImGui::DragFloat3("Up Axis", glm::value_ptr(component->upAxis), 0.01f);
	}
};

class DiffuseLightInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<DiffuseLightComponent>() == false) return;
		auto component = &entity->GetComponent<DiffuseLightComponent>();

		ImGui::SeparatorText("Diffuse Light");
	}
};