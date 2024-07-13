#pragma once
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "ParametersRenderer.h"
#include "AssetsDatabase.h"
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
#include "EngineShared/Components/MeshComponent.h"

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
	void RenderComponent(Ref<Entity> entity, UboDiffuseLightComponent& component);
	void RenderComponent(Ref<Entity> entity, MeshComponent& component);
};