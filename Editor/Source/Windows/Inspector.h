#pragma once

#include <imgui.h>

#include "AssetsDatabase.h"
#include "ComponentsRenderer.h"
#include "EngineShared/Ref.h"
#include "EngineShared/Entity.h"
#include "EngineShared/IWindow.h"

class Inspector : public IWindow
{
public:
	Inspector(Ref<AssetsDatabase> assetsDatabase);

	void Update() override;
	void Observe(Ref<Entity> entity);

private:
	URef<ComponentsRenderer> componentsRenderer;

	Ref<AssetsDatabase> assetsDatabase;
	Ref<Entity> observingEntity;

	void ShowAddComponentBtn();
};

