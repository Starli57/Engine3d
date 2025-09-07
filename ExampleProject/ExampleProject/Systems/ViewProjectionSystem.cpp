#include "ViewProjectionSystem.h"

#include "EngineCore/Components/CameraComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ShadowMapComponent.h"
#include "EngineCore/Components/UboWorldComponent.h"
#include "EngineCore/Profiler/Profiler.h"
#include "EngineCore/Utilities/MathUtility.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

ViewProjectionSystem::ViewProjectionSystem(const Ref<Ecs>& ecs, GLFWwindow* window)
{
	this->ecs = ecs;
	this->window = window;
}

void ViewProjectionSystem::Update(float deltaTime = 0)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	if (width == 0 || height == 0) return;

	Engine::Profiler::GetInstance().BeginSample("Camera ViewProjectionSystems");
	const auto screenAspectRatio = static_cast<float>(width) / static_cast<float>(height);

	const auto cameraEntities = ecs->registry->view<PositionComponent, RotationComponent, UboWorldComponent, CameraComponent>();
	for (const auto entity : cameraEntities)
	{
		auto& position = cameraEntities.get<PositionComponent>(entity).position;
		auto& rotation = cameraEntities.get<RotationComponent>(entity).rotation;
		auto& uboComponent = cameraEntities.get<UboWorldComponent>(entity);
		auto& cameraComponent = cameraEntities.get<CameraComponent>(entity);

		glm::vec3 direction;
		Engine::CalculateDirection(&direction, rotation);
		Engine::Normalize(&direction);
		
		uboComponent.view = lookAt(position, position + direction, cameraComponent.upAxis);
		auto projection = glm::perspective(glm::radians(cameraComponent.fov), 
			screenAspectRatio, cameraComponent.zNear, cameraComponent.zFar);

#if GLFW_INCLUDE_VULKAN
		projection[1][1] *= -1;
#endif
		
		uboComponent.projection = projection;
		uboComponent.position = position;
	}
	Engine::Profiler::GetInstance().EndSample();
}