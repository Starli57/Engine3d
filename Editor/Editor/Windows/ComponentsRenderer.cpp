
#include "ComponentsRenderer.h"

ComponentsRenderer::ComponentsRenderer(const Ref<ResourcesStorageVulkan>& resourcesStorage)
{
	this->resourcesStorage = resourcesStorage;
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
	RenderComponent<LightComponent>(entity);
	RenderComponent<UboModelComponent>(entity);
	RenderComponent<UboWorldComponent>(entity);
	RenderComponent<ShadowMapComponent>(entity);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, NameComponent& component) const
{
	parametersRenderer->RenderParameter(component.name);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, PositionComponent& component) const
{
	parametersRenderer->RenderParameter("Position", component.position);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, RotationComponent& component) const
{
	parametersRenderer->RenderParameter("Rotation", component.rotation);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, RotationVelocityComponent& component) const
{
	parametersRenderer->RenderParameter("Velocity", component.velocity);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, ScaleComponent& component) const
{
	parametersRenderer->RenderParameter("Scale", component.scale, 0, FLT_MAX);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, CameraComponent& component) const
{
	parametersRenderer->RenderParameter("FOV", component.fov, 30.0f, 120.0f);
	parametersRenderer->RenderParameter("ZNear", component.zNear);
	parametersRenderer->RenderParameter("ZFar", component.zFar);
	parametersRenderer->RenderParameter("Up Axis", component.upAxis);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, CameraFreeComponent& component) const
{
	parametersRenderer->RenderParameter("Movement speed", component.movementSpeed, 0, 10000);
	parametersRenderer->RenderParameter("Rotation speed", component.rotationSpeed, 0, 100);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, LightComponent& component) const
{
	parametersRenderer->RenderColorParameter("Light Color", component.color);
	parametersRenderer->RenderParameter("Intensity", component.intensity, 0, FLT_MAX);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, UboModelComponent& component) const
{
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, UboWorldComponent& component) const
{
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, MeshComponent& component) const
{
	auto max = static_cast<uint32_t>(resourcesStorage->MeshesCount() - 1);
	ImGui::Text(resourcesStorage->meshesPaths.at(component.meshIndex.value()).filename().string().c_str());
	parametersRenderer->RenderParameter("Mesh index", component.meshIndex, 0, max);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, MaterialComponent& component) const
{
	auto max = static_cast<uint32_t>(resourcesStorage->MaterialsCount() - 1);
	ImGui::Text(resourcesStorage->materialsPaths.at(component.materialIndex).filename().string().c_str());
	parametersRenderer->RenderParameter("Material index", component.materialIndex, 0U, max);
}

void ComponentsRenderer::RenderComponent(const Ref<Entity>&, ShadowMapComponent& component) const
{
	parametersRenderer->RenderParameter("FOV", component.fov, 30.0f, 120.0f);
	parametersRenderer->RenderParameter("ZNear", component.zNear);
	parametersRenderer->RenderParameter("ZFar", component.zFar);
	parametersRenderer->RenderParameter("Up Axis", component.upAxis);
}