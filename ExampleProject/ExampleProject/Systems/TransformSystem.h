#pragma once

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "EngineCore/Core/ISystem.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"

class TransformSystem : public ISystem
{
public:
	TransformSystem(const Ref<Ecs>& ecs);
	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};
