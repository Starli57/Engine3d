
#include "ComponentsRenderer.h"

ComponentsRenderer::ComponentsRenderer(const Ref<AssetsDatabase>& assetsDatabase)
{
	this->assetsDatabase = assetsDatabase;
	parametersRenderer = CreateRef<ParametersRenderer>();
}

void ComponentsRenderer::Update(const Ref<Entity>& entity)
{
	RenderComponent<NameComponent>(entity);
	RenderComponent<PositionComponent>(entity);
	RenderComponent<RotationComponent>(entity);
	RenderComponent<RotationVelocityComponent>(entity);
	RenderComponent<ScaleComponent>(entity);
	RenderComponent<MeshComponent>(entity);
	RenderComponent<MaterialComponent>(entity);
	RenderComponent<CameraComponent>(entity);
	RenderComponent<CameraFreeComponent>(entity);
	RenderComponent<UboDiffuseLightComponent>(entity);
	RenderComponent<UboModelComponent>(entity);
	RenderComponent<UboViewProjectionComponent>(entity);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, NameComponent& component) const
{
	parametersRenderer->RenderParameter(component.name);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, PositionComponent& component) const
{
	parametersRenderer->RenderParameter("Position", component.position, 0.1f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, RotationComponent& component) const
{
	parametersRenderer->RenderParameter("Rotation", component.rotation, 0.1f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, RotationVelocityComponent& component) const
{
	parametersRenderer->RenderParameter("Velocity", component.velocity, 0.1f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, ScaleComponent& component) const
{
	parametersRenderer->RenderParameter("Scale", component.scale, 0.01f, 0, FLT_MAX);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, CameraComponent& component) const
{
	parametersRenderer->RenderParameter("FOV", component.fov, 0.1f, 30.0f, 120.0f);
	parametersRenderer->RenderParameter("ZNear", component.zNear, 0.1f);
	parametersRenderer->RenderParameter("ZFar", component.zFar, 0.1f);
	parametersRenderer->RenderParameter("Up Axis", component.upAxis, 0.01f);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, CameraFreeComponent& component) const
{
	parametersRenderer->RenderParameter("Movement speed", component.movementSpeed, 1, 0, 10000);
	parametersRenderer->RenderParameter("Rotation speed", component.rotationSpeed, 0.2f, 0, 100);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, UboDiffuseLightComponent& component) const
{
	//	RenderParameter("Intensity", component->intensity, 0.01f, 0, FLT_MAX);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, UboModelComponent& component) const
{
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, UboViewProjectionComponent& component) const
{
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, MeshComponent& component) const
{
	auto max = static_cast<uint32_t>(assetsDatabase->MeshesCount() - 1);
	ImGui::Text(assetsDatabase->meshesPaths.at(component.meshIndex.value()).filename().string().c_str());
	parametersRenderer->RenderParameter("Mesh index", component.meshIndex, 1, 0, max);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, MaterialComponent& component) const
{
	auto max = static_cast<uint32_t>(assetsDatabase->MaterialsCount() - 1);
	ImGui::Text(assetsDatabase->materialsPaths.at(component.materialIndex).filename().string().c_str());
	parametersRenderer->RenderParameter("Material index", component.materialIndex, 1U, 0U, max);
}
