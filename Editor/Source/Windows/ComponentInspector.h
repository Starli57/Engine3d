#pragma once
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"
#include "EngineShared/Components/NameComponent.h"
#include "EngineShared/Components/PositionComponent.h"
#include "EngineShared/Components/RotationComponent.h"

class IComponentInspector
{
public:
	virtual void Update(Ref< Entity> entity) = 0;
};

class NameInspector : IComponentInspector
{
public:
	void Update(Ref<Entity> entity) override
	{
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
		auto component = &entity->GetComponent<RotationComponent>();

		ImGui::SeparatorText("Rotation Component");
		ImGui::DragFloat3("Rotation", glm::value_ptr(component->rotation), 0.1f);
	}
};