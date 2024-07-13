
#include "ComponentsRenderer.h"

ComponentsRenderer::ComponentsRenderer(Ref<AssetsDatabase> assetsDatabase)
{
	this->assetsDatabase = assetsDatabase;
	parametersRenderer = CreateRef<ParametersRenderer>();
}

void ComponentsRenderer::Update(Ref<Entity> entity)
{
	RenderComponent<NameComponent>(entity);
	RenderComponent<PositionComponent>(entity);
	RenderComponent<RotationComponent>(entity);
	RenderComponent<RotationVelocityComponent>(entity);
	RenderComponent<ScaleComponent>(entity);
	RenderComponent<MeshComponent>(entity);
	RenderComponent<CameraComponent>(entity);
	RenderComponent<UboDiffuseLightComponent>(entity);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, NameComponent& component)
{
	parametersRenderer->RenderParameter(component.name);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, PositionComponent& component)
{
	parametersRenderer->RenderParameter("Position", component.position, 0.1f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, RotationComponent& component)
{
	parametersRenderer->RenderParameter("Rotation", component.rotation, 0.1f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, RotationVelocityComponent& component)
{
	parametersRenderer->RenderParameter("Velocity", component.velocity, 0.1f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, ScaleComponent& component)
{
	parametersRenderer->RenderParameter("Scale", component.scale, 0.01f, 0, FLT_MAX);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, CameraComponent& component)
{
	parametersRenderer->RenderParameter("FOV", component.fov, 0.1f, 30.0f, 120.0f);
	parametersRenderer->RenderParameter("ZNear", component.zNear, 0.1f);
	parametersRenderer->RenderParameter("ZFar", component.zFar, 0.1f);
	parametersRenderer->RenderParameter("Look at", component.lookPoint);
	parametersRenderer->RenderParameter("Up Axis", component.upAxis, 0.01f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, UboDiffuseLightComponent& component)
{
	parametersRenderer->RenderParameter("Position", component.position, 0.25f);
	//	RenderParameter("Intensity", component->intensity, 0.01f, 0, FLT_MAX);
}

//looks like this component has a custom parameter like meshPath, because
//it's not enough to just show or change the path, 
//need to call specific engine methods to unload and load new mesh 
void ComponentsRenderer::RenderComponent(Ref<Entity> entity, MeshComponent& component)
{
	ImGui::Text("Mesh");

	if (component.meshPath.has_value())
	{
		auto filename = component.meshPath.value().filename();
		ImGui::SameLine();
		ImGui::Text(filename.string().c_str());
	}

	if (ImGui::Button("Change.."))
	{
		ImGui::OpenPopup("ChangePopup");
	}

	if (ImGui::BeginPopup("ChangePopup"))
	{
		for (auto it : assetsDatabase->meshesPaths)
		{
			if (ImGui::Selectable(it.first.c_str()))
			{
				component.meshPath = it.second;
			}
		}

		ImGui::EndPopup();
	}
}