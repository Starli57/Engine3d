#include "DirectionalShadowMapProjectionSystem.h"

#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ShadowMapComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Utilities/MathUtility.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

DirectionalShadowMapProjectionSystem::DirectionalShadowMapProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window)
    : ecs(ecs), window(window)
{
}

void DirectionalShadowMapProjectionSystem::Update(float deltaTime)
{
int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	if (width == 0 || height == 0) return;

	Engine::Profiler::GetInstance().BeginSample("Directional ShadowMap ProjectionSystems");
	const auto screenAspectRatio = static_cast<float>(width) / static_cast<float>(height);

	auto allProjections = ecs->registry->view<PositionComponent, RotationComponent, UboWorldComponent, ShadowMapComponent>();
	for (auto entity : allProjections)
	{
		auto& position = allProjections.get<PositionComponent>(entity).position;
		auto& rotation = allProjections.get<RotationComponent>(entity).rotation;
		auto& uboComponent = allProjections.get<UboWorldComponent>(entity);
		auto& shadowMapComponent = allProjections.get<ShadowMapComponent>(entity);
		
		glm::vec3 direction;
		Engine::CalculateDirection(&direction, rotation);
		Engine::Normalize(&direction);

		uboComponent.view = lookAt(position, position + direction, shadowMapComponent.upAxis);
		const auto projection = glm::perspective(glm::radians(shadowMapComponent.fov), screenAspectRatio, shadowMapComponent.zNear, shadowMapComponent.zFar);
		
		uboComponent.projection = projection;
		uboComponent.position = position;
	}
	Engine::Profiler::GetInstance().EndSample();
}
