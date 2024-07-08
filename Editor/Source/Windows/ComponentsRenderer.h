#pragma once
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "ParametersRenderer.h"
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

class ComponentsRenderer
{
public:

	ComponentsRenderer();
	void Update(Ref<Entity> entity);

private:

	Ref<ParametersRenderer> parametersRenderer;

	template <typename T>
	void RenderComponent(Ref<Entity> entity)
	{
		if (!entity->HasComponent<T>()) return;
		auto& component = entity->GetComponent<T>();

		ImGui::SeparatorText(typeid(T).name());
		RenderComponent(component);
	}

	void RenderComponent(NameComponent& component);
	void RenderComponent(PositionComponent& component);
	void RenderComponent(RotationComponent& component);
	void RenderComponent(RotationVelocityComponent& component);
	void RenderComponent(ScaleComponent& component);
	void RenderComponent(CameraComponent& component);
	void RenderComponent(UboDiffuseLightComponent& component);

};