
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
	RenderComponent<MaterialComponent>(entity);
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

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, MeshComponent& component)
{
	int max = assetsDatabase->MeshesCount() - 1;
	parametersRenderer->RenderParameter("Mesh index", component.meshIndex, 1, 0, max);
}

void ComponentsRenderer::RenderComponent(Ref<Entity> entity, MaterialComponent& component)
{
	uint32_t max = assetsDatabase->MaterialsCount() - 1;
	parametersRenderer->RenderParameter("Material index", component.materialIndex, 1U, 0U, max);
}
