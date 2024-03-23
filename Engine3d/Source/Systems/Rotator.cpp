#include "Pch.h"
#include "Rotator.h"

Rotator::Rotator(Ref<entt::registry> ecs) : ecs(ecs)
{
}

void Rotator::Update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	auto rotationComponents = ecs->view<RotationComponent>();
	for (auto entity : rotationComponents)
	{
		auto& rotationComponent = rotationComponents.get<RotationComponent>(entity);
		rotationComponent.rotation = glm::vec3(0.0f, 0.0f, 1.0f) * time * glm::radians(1800.0f);
	}
}
