#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "EngineShared/Entity.h"
#include "ISystem.h"
#include "EngineShared/Components/UboModelComponent.h"
#include "EngineShared/Components/PositionComponent.h"
#include "EngineShared/Components/RotationComponent.h"
#include "EngineShared/Components/ScaleComponent.h"
#include "EngineShared/Ref.h"

class TransformSystem : public ISystem
{
public:
	TransformSystem(Ref<entt::registry> ecs);
	virtual void Update(float deltaTime) override;

private:
	Ref<entt::registry> ecs;
};
