#include "RotateSystem.h"

#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/RotationVelocityComponent.h"
#include "EngineCore/Profiler/Profiler.h"

RotateSystem::RotateSystem(const Ref<Ecs>& ecs) : ecs(ecs)
{
}

void RotateSystem::Update(const float deltaTime)
{
	Engine::Profiler::GetInstance().BeginSample("RotateSystem");
	const auto rotationComponents = ecs->registry->view<RotationVelocityComponent, RotationComponent>();
	for (const auto entity : rotationComponents)
	{
		auto& rotationComponent = rotationComponents.get<RotationComponent>(entity);
		auto& rotationVelocityComponent = rotationComponents.get<RotationVelocityComponent>(entity);

		rotationComponent.rotation += rotationVelocityComponent.velocity * deltaTime;
	}
	Engine::Profiler::GetInstance().EndSample();
}
