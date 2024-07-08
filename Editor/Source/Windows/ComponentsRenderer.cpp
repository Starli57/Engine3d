
#include "ComponentsRenderer.h"

ComponentsRenderer::ComponentsRenderer()
{
	parametersRenderer = CreateRef<ParametersRenderer>();
}

void ComponentsRenderer::Update(Ref<Entity> entity)
{
	RenderComponent<NameComponent>(entity);
	RenderComponent<PositionComponent>(entity);
	RenderComponent<RotationComponent>(entity);
	RenderComponent<RotationVelocityComponent>(entity);
	RenderComponent<ScaleComponent>(entity);

	RenderComponent<CameraComponent>(entity);

	RenderComponent<UboDiffuseLightComponent>(entity);
}

void ComponentsRenderer::RenderComponent(NameComponent& component)
{
	parametersRenderer->RenderParameter(component.name);
}

void ComponentsRenderer::RenderComponent(PositionComponent& component)
{
	parametersRenderer->RenderParameter("Position", component.position, 0.1f);
}

void ComponentsRenderer::RenderComponent(RotationComponent& component)
{
	parametersRenderer->RenderParameter("Rotation", component.rotation, 0.1f);
}

void ComponentsRenderer::RenderComponent(RotationVelocityComponent& component)
{
	parametersRenderer->RenderParameter("Velocity", component.velocity, 0.1f);
}

void ComponentsRenderer::RenderComponent(ScaleComponent& component)
{
	parametersRenderer->RenderParameter("Scale", component.scale, 0.01f, 0, FLT_MAX);
}

void ComponentsRenderer::RenderComponent(CameraComponent& component)
{
	parametersRenderer->RenderParameter("FOV", component.fov, 0.1f, 30.0f, 120.0f);
	parametersRenderer->RenderParameter("ZNear", component.zNear, 0.1f);
	parametersRenderer->RenderParameter("ZFar", component.zFar, 0.1f);
	parametersRenderer->RenderParameter("Look at", component.lookPoint);
	parametersRenderer->RenderParameter("Up Axis", component.upAxis, 0.01f);
}

void ComponentsRenderer::RenderComponent(UboDiffuseLightComponent& component)
{
	parametersRenderer->RenderParameter("Position", component.position, 0.25f);
	//	RenderParameter("Intensity", component->intensity, 0.01f, 0, FLT_MAX);
}