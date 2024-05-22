#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "SharedLib/Entity.h"
#include "ISystem.h"
#include "SharedLib/Components/UboModelComponent.h"
#include "SharedLib/Components/PositionComponent.h"
#include "SharedLib/Components/RotationComponent.h"
#include "SharedLib/Components/ScaleComponent.h"
#include "SharedLib/Ref.h"

class TransformSystem : public ISystem
{
public:
	TransformSystem(Ref<entt::registry> ecs);
	virtual void Update(float deltaTime) override;

private:
	Ref<entt::registry> ecs;
};
