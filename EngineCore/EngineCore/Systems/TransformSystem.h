#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "EngineCore/Systems/ISystem.h"
#include "EngineCore/Components/UboModelComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"
#include "EngineCore/Components/ScaleComponent.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Ecs.h"

class TransformSystem : public ISystem
{
public:
	TransformSystem(Ref<Ecs> ecs);
	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
};
