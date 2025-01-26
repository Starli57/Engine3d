#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Input.h"
#include "EngineCore/Systems/ISystem.h"
#include "EngineCore/Components/CameraOrbitComponent.h"
#include "EngineCore/Components/PositionComponent.h"

class CameraOrbitSystem : ISystem
{
public:
	CameraOrbitSystem(const Ref<Ecs>& ecs, const Ref<Input>& input);
	~CameraOrbitSystem();

	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	Ref<Input> input;
};

