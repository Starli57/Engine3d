#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Input.h"
#include "EngineCore/Systems/ISystem.h"
#include "EngineCore/Components/CameraFreeComponent.h"
#include "EngineCore/Components/PositionComponent.h"
#include "EngineCore/Components/RotationComponent.h"

class CameraFlySystem : ISystem
{
public:
	CameraFlySystem(const Ref<Ecs>& ecs, const Ref<Input>& input);
	~CameraFlySystem();

	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	Ref<Input> input;
};