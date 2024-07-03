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
#include "EngineShared/Components/UboDiffuseLightComponent.h"

class ComponentInspector
{
public:
	void Update(Ref<Entity> entity)
	{
		RenderComponent<NameComponent>(entity);
		RenderComponent<PositionComponent>(entity);
		RenderComponent<RotationComponent>(entity);
		RenderComponent<RotationVelocityComponent>(entity);
		RenderComponent<ScaleComponent>(entity);

		RenderComponent<CameraComponent>(entity);

		RenderComponent<UboDiffuseLightComponent>(entity);
	}

private:
	template <typename T>
	void RenderComponent(Ref<Entity> entity)
	{
		if (!entity->HasComponent<T>()) return;
		auto& component = entity->GetComponent<T>();

		ImGui::SeparatorText(typeid(T).name());
		RenderComponent(component);
	}

	void RenderComponent(NameComponent& component) 
	{
		RenderParameter(component.name);
	}

	void RenderComponent(PositionComponent& component) 
	{
		RenderParameter("Position", component.position, 0.1f);
	}

	void RenderComponent(RotationComponent& component) 
	{
		RenderParameter("Rotation", component.rotation, 0.1f);
	}

	void RenderComponent(RotationVelocityComponent& component)
	{
		RenderParameter("Velocity", component.velocity, 0.1f);
	}

	void RenderComponent(ScaleComponent& component) 
	{
		RenderParameter("Scale", component.scale, 0.01f, 0, FLT_MAX);
	}

	void RenderComponent(CameraComponent& component) 
	{
		RenderParameter("FOV", component.fov, 0.1f, 30.0f, 120.0f);
		RenderParameter("ZNear", component.zNear, 0.1f);
		RenderParameter("ZFar", component.zFar, 0.1f);
		RenderParameter("Look at", component.lookPoint);
		RenderParameter("Up Axis", component.upAxis, 0.01f);
	}

	void RenderComponent(UboDiffuseLightComponent& component)
	{
		RenderParameter("Position", component.position, 0.25f);
		//	RenderParameter("Intensity", component->intensity, 0.01f, 0, FLT_MAX);
	}

	void RenderParameter(const char* label, int& parameter, int step = 1)
	{
		ImGui::InputInt(label, &parameter, step);
	}

	void RenderParameter(const char* label, float& parameter, float v_speed = 0.1f, float v_min = 0, float v_max = 0)
	{
		ImGui::DragFloat(label, &parameter, v_speed, v_min, v_max);
	}

	void RenderParameter(const char* label, glm::vec2& parameter, float v_speed = 0.1f, float v_min = 0, float v_max = 0)
	{
		ImGui::DragFloat2(label, glm::value_ptr(parameter), v_speed, v_min, v_max);
	}

	void RenderParameter(const char* label, glm::vec3& parameter, float v_speed = 0.1f, float v_min = 0, float v_max = 0)
	{
		ImGui::DragFloat3(label, glm::value_ptr(parameter), v_speed, v_min, v_max);
	}

	void RenderParameter(std::string& parameter)
	{
		ImGui::Text(parameter.c_str());
	}
};