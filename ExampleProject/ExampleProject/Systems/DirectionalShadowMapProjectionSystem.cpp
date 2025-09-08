#include "DirectionalShadowMapProjectionSystem.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ShadowMapComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Utilities/CameraUtilities.h"
#include "EngineCore/Utilities/MathUtility.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

DirectionalShadowMapProjectionSystem::DirectionalShadowMapProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window)
    : ecs(ecs), window(window)
{
}

void DirectionalShadowMapProjectionSystem::Update(float deltaTime)
{
	const UboWorldComponent* cameraUbo = nullptr;
	const CameraComponent* cameraComponent = nullptr;
	
	const auto cameraEntities = ecs->registry->view<PositionComponent, RotationComponent, UboWorldComponent, CameraComponent>();
	for (auto entity : cameraEntities)
	{
		cameraUbo = &cameraEntities.get<UboWorldComponent>(entity);
		cameraComponent = &cameraEntities.get<CameraComponent>(entity);
		break;
	}
	if (cameraComponent == nullptr) return;

	Engine::Profiler::GetInstance().BeginSample("Directional Shadow Map Projection System");

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	if (width == 0 || height == 0) return;

	const auto screenAspectRatio = static_cast<float>(width) / static_cast<float>(height);
	
	auto allProjections = ecs->registry->view<PositionComponent, RotationComponent, UboWorldComponent, ShadowMapComponent>();
	for (auto entity : allProjections)
	{
		auto& shadowCasterPosition = allProjections.get<PositionComponent>(entity).position;
		auto& rotation = allProjections.get<RotationComponent>(entity).rotation;
		auto& uboComponent = allProjections.get<UboWorldComponent>(entity);
		auto& shadowMapComponent = allProjections.get<ShadowMapComponent>(entity);
		
		const auto cameraProjection = glm::perspective(glm::radians(cameraComponent->fov), screenAspectRatio, shadowMapComponent.zNear, shadowMapComponent.zFar);
		const auto corners = Engine::GetFrustumCornersWorldSpace(cameraProjection * cameraUbo->view);

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& corner : corners)
		{
			center += glm::vec3(corner);
		}
		center /= corners.size();
		
		glm::vec3 direction;
		Engine::CalculateDirection(&direction, rotation);
		Engine::Normalize(&direction);

		uboComponent.view = lookAt(center, center + direction, shadowMapComponent.upAxis);
		uboComponent.position = shadowCasterPosition;
		
		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();
		for (const auto& corner : corners)
		{
			const auto trf = uboComponent.view * corner;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}
		
		if (minZ < 0) minZ *= shadowMapComponent.farObjectProjectionMultiplier;
		else minZ /= shadowMapComponent.farObjectProjectionMultiplier;
		
		if (maxZ < 0) maxZ /= shadowMapComponent.farObjectProjectionMultiplier;
		else maxZ *= shadowMapComponent.farObjectProjectionMultiplier;
		
		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
		uboComponent.projection = lightProjection;
	}
	Engine::Profiler::GetInstance().EndSample();
}
