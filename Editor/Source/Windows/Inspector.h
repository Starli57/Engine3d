#pragma once

#include <imgui.h>

#include "ComponentInspector.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"
#include "EngineShared/IWindow.h"

class Inspector : public IWindow
{
public:
	void Update() override;
	void Observe(Ref<Entity> entity);

private:
	Ref<Entity> observingEntity;
};

