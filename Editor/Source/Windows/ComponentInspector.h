#pragma once
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"
#include "EngineShared/Components/NameComponent.h"
#include "EngineShared/Components/PositionComponent.h"
#include "EngineShared/Components/RotationComponent.h"
#include "EngineShared/Components/CameraComponent.h"

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

		ImGui::SeparatorText("Name Component");
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

		ImGui::SeparatorText("Position Component");
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

		ImGui::SeparatorText("Rotation Component");
		ImGui::DragFloat3("Rotation", glm::value_ptr(component->rotation), 0.1f);
	}
};

class CameraInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
		if (entity->HasComponent<CameraComponent>() == false) return;
		auto component = &entity->GetComponent<CameraComponent>();

		ImGui::SeparatorText("Camera Component");
		ImGui::DragFloat("FOV", &component->fov, 0.1f, 30, 120);
		ImGui::DragFloat("ZNear", &component->zNear, 0.1f);
		ImGui::DragFloat("ZFar", &component->zFar, 0.1f);
		ImGui::DragFloat3("Center", glm::value_ptr(component->centerPoint));
		ImGui::DragFloat3("Up Axis", glm::value_ptr(component->upAxis), 0.01f);
	}
};