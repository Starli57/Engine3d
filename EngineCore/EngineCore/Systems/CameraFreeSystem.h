#pragma once

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/Input.h"
#include "EngineCore/Systems/ISystem.h"
#include "EngineCore/Components/CameraFreeComponent.h"
#include "EngineCore/Components/PositionComponent.h"

class CameraFreeSystem : ISystem
{
public:
	CameraFreeSystem(Ref<Ecs> ecs, Ref<Input> input);
	~CameraFreeSystem();

	virtual void Update(float deltaTime) override;

private:
	Ref<Ecs> ecs;
	Ref<Input> input;
};