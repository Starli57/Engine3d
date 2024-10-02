#pragma once
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "ParametersRenderer.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/AssetsManagement/AssetsDatabase.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/CameraFreeComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Components/UboDiffuseLightComponent.h"
#include "EngineCore/Components/MeshComponent.h"
#include "EngineCore/Components/MaterialComponent.h"

class ComponentsRenderer
{
public:

	ComponentsRenderer(Ref<AssetsDatabase> assetsDatabase);
	void Update(Ref<Entity> entity);

private:

	Ref<AssetsDatabase> assetsDatabase;
	Ref<ParametersRenderer> parametersRenderer;

	template <typename T>
	void RenderComponent(Ref<Entity> entity)
	{
		if (!entity->HasComponent<T>()) return;
		auto& component = entity->GetComponent<T>();

		ImGui::SeparatorText(typeid(T).name());
		RenderComponent(entity, component);
	}

	void RenderComponent(Ref<Entity> entity, NameComponent& component);
	void RenderComponent(Ref<Entity> entity, PositionComponent& component);
	void RenderComponent(Ref<Entity> entity, RotationComponent& component);
	void RenderComponent(Ref<Entity> entity, RotationVelocityComponent& component);
	void RenderComponent(Ref<Entity> entity, ScaleComponent& component);
	void RenderComponent(Ref<Entity> entity, CameraComponent& component);
	void RenderComponent(Ref<Entity> entity, CameraFreeComponent& component);
	void RenderComponent(Ref<Entity> entity, UboDiffuseLightComponent& component);
	void RenderComponent(Ref<Entity> entity, MeshComponent& component);
	void RenderComponent(Ref<Entity> entity, MaterialComponent& component);
};